#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <raylib.h>
#include <raymath.h>

#include "env.h"
#include "plug.h"

#define COMMON_IMPLEMENTATION
#include "common.h"
#include "cJSON.c"

#define PLUG(name, ret, ...) ret name(__VA_ARGS__);
LIST_OF_PLUGS
#undef PLUG

#define NODE_RADIUS 35.0f
#define NODE_SPACING 100.0f

typedef struct {
    int id;
    int value;
    bool highlighted;
    int size;
    int left_id;
    int right_id;
    int x;
    int y;
} NodeState;

typedef struct {
    NodeState *nodes;
    size_t num_nodes;
    int *call_stack;
    size_t call_stack_size;
    int line;
} TreeState;

typedef struct {
    TreeState *states;
    size_t num_states;
} Trace;

typedef struct {
    size_t size;
    Trace trace;
    CommonState common;
    Subprocess sp;
    SourceView sv;
    bool show_source;
    char buffer[4 * 1024 * 1024];
    size_t buffer_len;
} Plug;

static Plug *p = NULL;

static void free_trace_data(Trace *trace) {
    if (trace->states) {
        for (size_t i = 0; i < trace->num_states; ++i) {
            if (trace->states[i].nodes) free(trace->states[i].nodes);
            if (trace->states[i].call_stack) free(trace->states[i].call_stack);
        }
        free(trace->states);
        trace->states = NULL;
    }
    trace->num_states = 0;
}

static void parse_state_json(cJSON *json) {
    if (p->trace.num_states == 0) {
        p->trace.states = malloc(sizeof(TreeState));
    } else {
        p->trace.states = realloc(p->trace.states, sizeof(TreeState) * (p->trace.num_states + 1));
    }
    TreeState *state = &p->trace.states[p->trace.num_states];
    memset(state, 0, sizeof(TreeState));

    cJSON *nodes_json = cJSON_GetObjectItem(json, "nodes");
    state->num_nodes = cJSON_GetArraySize(nodes_json);
    state->nodes = malloc(sizeof(NodeState) * state->num_nodes);
    int j = 0;
    cJSON *node_json;
    cJSON_ArrayForEach(node_json, nodes_json) {
        NodeState *node = &state->nodes[j];
        node->id = cJSON_GetObjectItem(node_json, "id")->valueint;
        node->value = cJSON_GetObjectItem(node_json, "value")->valueint;
        node->highlighted = cJSON_IsTrue(cJSON_GetObjectItem(node_json, "highlighted"));
        node->size = cJSON_GetObjectItem(node_json, "size")->valueint;
        node->left_id = cJSON_GetObjectItem(node_json, "left_id")->valueint;
        node->right_id = cJSON_GetObjectItem(node_json, "right_id")->valueint;
        node->x = cJSON_GetObjectItem(node_json, "x")->valueint;
        node->y = cJSON_GetObjectItem(node_json, "y")->valueint;
        j++;
    }
    cJSON *call_stack_json = cJSON_GetObjectItem(json, "call_stack");
    state->call_stack_size = cJSON_GetArraySize(call_stack_json);
    state->call_stack = malloc(sizeof(int) * state->call_stack_size);
    int k = 0;
    cJSON *stack_elem_json;
    cJSON_ArrayForEach(stack_elem_json, call_stack_json) {
        state->call_stack[k] = stack_elem_json->valueint;
        k++;
    }
    state->line = cJSON_GetObjectItem(json, "line")->valueint;
    p->trace.num_states++;
}

void plug_init(void) {
    p = malloc(sizeof(*p));
    assert(p != NULL);
    memset(p, 0, sizeof(*p));
    p->size = sizeof(*p);
    common_init(&p->common);
    common_load_assets(&p->common);
    p->show_source = false;
    
    char *argv[] = {"./a.out", NULL};
    const char *src_file = "main.cpp";
    if (FileExists("../a.out")) {
        argv[0] = "../a.out";
        src_file = "../main.cpp";
    }
    p->sp = subprocess_start(argv[0], argv);
    source_view_load(&p->sv, src_file);
}

void *plug_pre_reload(void) {
    common_unload_assets(&p->common);
    free_trace_data(&p->trace);
    subprocess_stop(&p->sp);
    source_view_unload(&p->sv);
    common_reset(&p->common);
    return p;
}

void plug_post_reload(void *state) {
    p = state;
    if (p->size < sizeof(*p)) {
        p = realloc(p, sizeof(*p));
        p->size = sizeof(*p);
    }
    common_load_assets(&p->common);
    
    char *argv[] = {"./a.out", NULL};
    const char *src_file = "main.cpp";
    if (FileExists("../a.out")) {
        argv[0] = "../a.out";
        src_file = "../main.cpp";
    }
    p->sp = subprocess_start(argv[0], argv);
    source_view_load(&p->sv, src_file);
}

void plug_reset(void) {
    common_reset(&p->common);
}

static NodeState* find_node_state(TreeState *tree_state, int id) {
    for (size_t i = 0; i < tree_state->num_nodes; ++i) {
        if (tree_state->nodes[i].id == id) return &tree_state->nodes[i];
    }
    return NULL;
}

void plug_update(Env env) {
    // Read from subprocess
    char chunk[4096];
    while (true) {
        ssize_t n = subprocess_read(&p->sp, chunk, sizeof(chunk));
        if (n <= 0) break;

        if (p->buffer_len + n < sizeof(p->buffer)) {
            memcpy(p->buffer + p->buffer_len, chunk, n);
            p->buffer_len += n;
            p->buffer[p->buffer_len] = 0;

            // Process lines
            char *start = p->buffer;
            char *end;
            while ((end = strchr(start, '\n')) != NULL) {
                *end = 0; // Null-terminate the line
                if (strlen(start) > 0) {
                    cJSON *json = cJSON_Parse(start);
                    if (json) {
                        parse_state_json(json);
                        cJSON_Delete(json);
                    }
                }
                start = end + 1;
            }

            // Move remaining data to front
            size_t remaining = p->buffer_len - (start - p->buffer);
            memmove(p->buffer, start, remaining);
            p->buffer_len = remaining;
        } else {
             break;
        }
    }

    float intro_t = common_update_logic(&p->common, env, p->trace.num_states);
    if (IsKeyPressed(KEY_S)) p->show_source = !p->show_source;

    float t = p->common.transition_time / ANIMATION_DURATION;

    ClearBackground(COLOR_BACKGROUND);
    if (p->trace.num_states == 0) return;

    TreeState *curr = &p->trace.states[p->common.current_state];
    TreeState *next = ((size_t)p->common.current_state < p->trace.num_states - 1) ? &p->trace.states[p->common.current_state + 1] : curr;

    int min_x = 0, max_x = 0, min_y = 0, max_y = 0;
    if (curr->num_nodes > 0) {
        min_x = curr->nodes[0].x; max_x = curr->nodes[0].x;
        min_y = curr->nodes[0].y; max_y = curr->nodes[0].y;
        for (size_t i = 1; i < curr->num_nodes; ++i) {
            if (curr->nodes[i].x < min_x) min_x = curr->nodes[i].x;
            if (curr->nodes[i].x > max_x) max_x = curr->nodes[i].x;
            if (curr->nodes[i].y < min_y) min_y = curr->nodes[i].y;
            if (curr->nodes[i].y > max_y) max_y = curr->nodes[i].y;
        }
    }
    Vector2 center = {(min_x + max_x) / 2.0f * NODE_SPACING, (min_y + max_y) / 2.0f * NODE_SPACING};

    Camera2D camera = { .target = center, .offset = {env.screen_width/2.0f, env.screen_height/2.0f} };
    common_update_interactivity(&p->common, env, &camera, true);
    camera.target = Vector2Add(center, p->common.camera_pan);

    BeginMode2D(camera);
    {
        for (size_t i = 0; i < curr->num_nodes; i++) {
            NodeState *node = &curr->nodes[i];
            Vector2 start_pos = {node->x * NODE_SPACING, node->y * NODE_SPACING};
            if (node->left_id != -1) {
                NodeState *lc = find_node_state(curr, node->left_id);
                if (lc) DrawLineEx(start_pos, (Vector2){lc->x * NODE_SPACING, lc->y * NODE_SPACING}, 3.0f, ColorAlpha(LIGHTGRAY, intro_t));
            }
            if (node->right_id != -1) {
                NodeState *rc = find_node_state(curr, node->right_id);
                if (rc) DrawLineEx(start_pos, (Vector2){rc->x * NODE_SPACING, rc->y * NODE_SPACING}, 3.0f, ColorAlpha(LIGHTGRAY, intro_t));
            }
        }

        for (size_t i = 0; i < curr->num_nodes; i++) {
            NodeState *node = &curr->nodes[i];
            NodeState *next_node = find_node_state(next, node->id);
            if (!next_node) next_node = node;
            Vector2 pos = {node->x * NODE_SPACING, node->y * NODE_SPACING};
            Color color = common_color_lerp(node->highlighted ? COLOR_MILD_GOLD : COLOR_OFF_WHITE, next_node->highlighted ? COLOR_MILD_GOLD : COLOR_OFF_WHITE, t);
            DrawCircleV(pos, NODE_RADIUS, ColorAlpha(color, intro_t));
            char buf[32]; snprintf(buf, sizeof(buf), "%d", node->value);
            Vector2 ts = MeasureTextEx(p->common.font, buf, 32, 1);
            DrawTextEx(p->common.font, buf, (Vector2){pos.x - ts.x / 2, pos.y - ts.y / 2 - 10}, 32, 1, ColorAlpha(COLOR_BACKGROUND, intro_t));
            if (node->size != 0) snprintf(buf, sizeof(buf), "%d", node->size);
            else               snprintf(buf, sizeof(buf), "?");
            ts = MeasureTextEx(p->common.font, buf, 18, 1);
            DrawTextEx(p->common.font, buf, (Vector2){pos.x - ts.x / 2, pos.y + 10}, 18, 1, ColorAlpha(COLOR_BACKGROUND, intro_t*0.7f));
            for (size_t k = 0; k < curr->call_stack_size; ++k) {
                if (curr->call_stack[k] == node->id) {
                    float stack_alpha = (k == curr->call_stack_size - 1) ? 1.0f : 0.6f;
                    DrawCircle(pos.x + NODE_RADIUS - 5, pos.y - NODE_RADIUS + 5, 6, ColorAlpha(COLOR_MILD_BLUE, intro_t * stack_alpha));
                }
            }
        }

        int head_id = (curr->call_stack_size > 0) ? curr->call_stack[curr->call_stack_size - 1] : -1;
        NodeState *hn = find_node_state(curr, head_id);
        if (hn) {
            Vector2 target = {hn->x * NODE_SPACING, hn->y * NODE_SPACING};
            if (p->common.head_pos.x < -0.5e6) p->common.head_pos = target;
            else p->common.head_pos = Vector2Lerp(p->common.head_pos, target, 12.0f * GetFrameTime());
            DrawRing(p->common.head_pos, NODE_RADIUS + 5.0f, NODE_RADIUS + 15.0f, 0, 360.0f * intro_t, 0, COLOR_HEAD);
        }
    }
    EndMode2D();

    if (p->common.show_internals) {
        common_render_internals_begin(&p->common, env, "Tree Call Stack");
        float pw = env.screen_width * 0.3f; float xo = env.screen_width - pw; float pad = 25.0f; float y = pad + 60.0f;
        if (curr->call_stack_size == 0) {
            DrawTextEx(p->common.font, "Empty", (Vector2){xo + pad + 10, y}, 24, 1, GRAY);
        } else {
            for (size_t i = 0; i < curr->call_stack_size; ++i) {
                //char buf[64]; snprintf(buf, sizeof(buf), "[Level %zu]: Node %d (dp=%d)", i, curr->call_stack[i], find_node_state(curr, curr->call_stack[i])->dp);
                char buf[64]; snprintf(buf, sizeof(buf), "Level %zu: Node %d", i, find_node_state(curr, curr->call_stack[i])->value);
                DrawTextEx(p->common.font, buf, (Vector2){xo + pad + 10, y}, 24, 1, ColorAlpha(COLOR_MILD_BLUE, intro_t));
                y += 28.0f;
            }
        }
    }
    
    if (p->show_source) {
        float sw = env.screen_width * 0.35f; // Source width
        float sx = 0; // Left side
        Rectangle bounds = {sx, 0, sw, (float)env.screen_height};
        source_view_render(&p->sv, &p->common, env, curr->line, bounds);
        DrawLineEx((Vector2){sw, 0}, (Vector2){sw, (float)env.screen_height}, 2.0f, ColorAlpha(WHITE, intro_t));
    }
}

bool plug_finished(void) { return false; }
