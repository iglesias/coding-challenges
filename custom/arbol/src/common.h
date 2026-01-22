#ifndef COMMON_H_
#define COMMON_H_

#include <raylib.h>
#include <raymath.h>
#include "env.h"
#include "interpolators.h"

// Subprocess / Streaming Support
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

typedef struct {
    pid_t pid;
    int fd; // Read end of the pipe
} Subprocess;

Subprocess subprocess_start(const char *command_path, char *const argv[]);
ssize_t subprocess_read(Subprocess *sp, char *buf, size_t count);
void subprocess_stop(Subprocess *sp);

// Unified Palette
#define COLOR_BACKGROUND (Color){ 24, 24, 24, 255 }
#define COLOR_OFF_WHITE  (Color){ 230, 230, 230, 255 }
#define COLOR_MILD_GREEN (Color){ 119, 184, 119, 255 }
#define COLOR_MILD_BLUE  (Color){ 119, 158, 204, 255 }
#define COLOR_MILD_GOLD  (Color){ 255, 220, 119, 255 }
#define COLOR_HEAD       ColorFromHSV(200, 0.8, 0.8)

#define INTRO_DURATION 1.0f
#define ANIMATION_DURATION 0.2f

typedef struct {
    float camera_zoom;
    Vector2 camera_pan;
    bool show_internals;
    float intro_time;
    float transition_time;
    int current_state;
    Font font;
    Vector2 head_pos;
    float step_repeat_timer;
} CommonState;

typedef struct {
    char **lines;
    bool *is_dimmed;
    int *mapped_lines;
    size_t count;
} SourceView;

void source_view_load(SourceView *sv, const char *filename);
void source_view_unload(SourceView *sv);
void source_view_render(SourceView *sv, CommonState *s, Env env, int current_line, Rectangle bounds);

void common_init(CommonState *s);
void common_reset(CommonState *s);
void common_load_assets(CommonState *s);
void common_unload_assets(CommonState *s);
float common_update_logic(CommonState *s, Env env, size_t num_states); // returns intro_t
void common_update_interactivity(CommonState *s, Env env, Camera2D *camera, bool allow_panning);
void common_render_internals_begin(CommonState *s, Env env, const char *title);

static inline Color common_color_lerp(Color start, Color end, float t) {
    return (Color){
        (unsigned char)Lerp(start.r, end.r, t),
        (unsigned char)Lerp(start.g, end.g, t),
        (unsigned char)Lerp(start.b, end.b, t),
        (unsigned char)Lerp(start.a, end.a, t)
    };
}

#ifdef COMMON_IMPLEMENTATION

void common_init(CommonState *s) {
    memset(s, 0, sizeof(*s));
    s->camera_zoom = 1.0f;
    common_reset(s);
}

void common_reset(CommonState *s) {
    s->current_state = 0;
    s->transition_time = 0.0f;
    s->intro_time = 0.0f;
    s->head_pos = (Vector2){-1e6, -1e6};
    s->show_internals = true;
    s->camera_pan = (Vector2){0, 0};
    s->step_repeat_timer = 0.0f;
}

void common_load_assets(CommonState *s) {
    // High resolution font to prevent blurriness
    s->font = LoadFontEx("./assets/fonts/iosevka-regular.ttf", 128, NULL, 0);
    if (s->font.texture.id == 0) {
        // Fallback if not run from panim/ directory
        s->font = LoadFontEx("../../assets/fonts/iosevka-regular.ttf", 128, NULL, 0);
    }
    SetTextureFilter(s->font.texture, TEXTURE_FILTER_BILINEAR);
}

void common_unload_assets(CommonState *s) {
    UnloadFont(s->font);
}

float common_update_logic(CommonState *s, Env env, size_t num_states) {
    float real_dt = GetFrameTime();

    if (IsKeyPressed(KEY_I)) s->show_internals = !s->show_internals;

    if (s->intro_time < INTRO_DURATION) {
        s->intro_time += real_dt;
        if (s->intro_time > INTRO_DURATION) s->intro_time = INTRO_DURATION;
    } else {
        float repeat_delay = 0.4f;
        float repeat_rate = 0.06f;

        bool step_forward = IsKeyPressed(KEY_RIGHT);
        bool step_backward = IsKeyPressed(KEY_LEFT);

        if (step_forward) {
            if ((size_t)s->current_state < num_states - 1) {
                s->current_state++;
                s->transition_time = 0.0f;
            }
            s->step_repeat_timer = repeat_delay;
        } else if (step_backward) {
            if (s->current_state > 0) {
                s->current_state--;
                s->transition_time = 0.0f;
            }
            s->step_repeat_timer = repeat_delay;
        } else if (IsKeyDown(KEY_RIGHT)) {
            s->step_repeat_timer -= real_dt;
            if (s->step_repeat_timer <= 0) {
                if ((size_t)s->current_state < num_states - 1) {
                    s->current_state++;
                    s->transition_time = 0.0f;
                }
                s->step_repeat_timer = repeat_rate;
            }
        } else if (IsKeyDown(KEY_LEFT)) {
            s->step_repeat_timer -= real_dt;
            if (s->step_repeat_timer <= 0) {
                if (s->current_state > 0) {
                    s->current_state--;
                    s->transition_time = 0.0f;
                }
                s->step_repeat_timer = repeat_rate;
            }
        }

        if (num_states > 1 && !IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT)) {
            s->transition_time += env.delta_time;
            if (s->transition_time >= ANIMATION_DURATION) {
                s->transition_time = 0.0f;
                if ((size_t)s->current_state < num_states - 1) s->current_state++;
            }
        }
    }

    return smoothstep(s->intro_time / INTRO_DURATION);
}

void common_update_interactivity(CommonState *s, Env env, Camera2D *camera, bool allow_panning) {
    (void)env;
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        s->camera_zoom += wheel * 0.1f;
        if (s->camera_zoom < 0.1f) s->camera_zoom = 0.1f;
    }

    if (allow_panning && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 delta = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f / s->camera_zoom);
        s->camera_pan = Vector2Add(s->camera_pan, delta);
    }

    if (s->show_internals) {
        camera->offset.x -= env.screen_width * 0.15f;
    }
    camera->zoom = s->camera_zoom;
}

void common_render_internals_begin(CommonState *s, Env env, const char *title) {
    float intro_t = smoothstep(s->intro_time / INTRO_DURATION);
    float pw = env.screen_width * 0.3f;
    float xo = env.screen_width - pw;
    DrawRectangle(xo, 0, pw, env.screen_height, ColorAlpha(DARKGRAY, 0.85f * intro_t));
    DrawLineEx((Vector2){xo, 0}, (Vector2){xo, (float)env.screen_height}, 2.0f, ColorAlpha(WHITE, intro_t));

    float pad = 25.0f;
    DrawTextEx(s->font, title, (Vector2){xo + pad, pad}, 32, 1, COLOR_MILD_GOLD);
}

Subprocess subprocess_start(const char *command_path, char *const argv[]) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return (Subprocess){0};
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return (Subprocess){0};
    }

    if (pid == 0) {
        // Child
        close(pipefd[0]); // Close read end
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(pipefd[1]); // Close original write end

        // Execute command
        execvp(command_path, argv);
        perror("execvp");
        exit(1);
    } else {
        // Parent
        close(pipefd[1]); // Close write end
        
        // Set read end to non-blocking
        int flags = fcntl(pipefd[0], F_GETFL, 0);
        fcntl(pipefd[0], F_SETFL, flags | O_NONBLOCK);

        return (Subprocess){ .pid = pid, .fd = pipefd[0] };
    }
}

ssize_t subprocess_read(Subprocess *sp, char *buf, size_t count) {
    if (sp->fd <= 0) return -1;
    return read(sp->fd, buf, count);
}

void subprocess_stop(Subprocess *sp) {
    if (sp->pid > 0) {
        kill(sp->pid, SIGTERM);
        waitpid(sp->pid, NULL, 0);
    }
    if (sp->fd > 0) {
        close(sp->fd);
    }
    sp->pid = 0;
    sp->fd = 0;
}

static bool is_noise_line(const char *line) {
    while (*line && (*line == ' ' || *line == '\t')) line++;
    if (*line == 0) return true; // Empty
    if (*line == '{' && *(line+1) == 0) return true; // Just opening brace
    if (*line == '}' && *(line+1) == 0) return true; // Just closing brace
    return false;
}

void source_view_load(SourceView *sv, const char *filename) {
    memset(sv, 0, sizeof(*sv));
    FILE *f = fopen(filename, "r");
    if (!f) return;

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, f)) != -1) {
        sv->lines = realloc(sv->lines, sizeof(char*) * (sv->count + 1));
        sv->is_dimmed = realloc(sv->is_dimmed, sizeof(bool) * (sv->count + 1));
        sv->mapped_lines = realloc(sv->mapped_lines, sizeof(int) * (sv->count + 1));
        
        // Remove newline
        if (read > 0 && line[read - 1] == '\n') line[read - 1] = '\0';
        sv->lines[sv->count] = strdup(line);
        
        // Heuristic for instrumentation lines
        sv->is_dimmed[sv->count] = false;
        if (strstr(line, "Trace::") || strstr(line, "TraceDijkstra::") || 
            strstr(line, "->highlight") || strstr(line, "->unhighlight") || 
            strstr(line, "->set_dp") || strstr(line, "->set_left") || 
            strstr(line, "->set_right") || strstr(line, "register_node") || 
            strstr(line, "register_edge") || strstr(line, "snapshot") ||
            strstr(line, "CallGuard") || strstr(line, "PRECONDITION")) {
            sv->is_dimmed[sv->count] = true;
        }

        // Initialize mapped lines to identity
        sv->mapped_lines[sv->count] = sv->count;
        sv->count++;
    }
    
    // Pass 2: Map dimmed lines backwards
    for (size_t i = 0; i < sv->count; ++i) {
        if (sv->is_dimmed[i]) {
            int candidate = (int)i - 1;
            while (candidate >= 0) {
                if (!sv->is_dimmed[candidate] && !is_noise_line(sv->lines[candidate])) {
                    break;
                }
                candidate--;
            }
            if (candidate >= 0) {
                sv->mapped_lines[i] = candidate;
            }
        }
    }

    if (line) free(line);
    fclose(f);
}

void source_view_unload(SourceView *sv) {
    if (sv->lines) {
        for (size_t i = 0; i < sv->count; ++i) {
            free(sv->lines[i]);
        }
        free(sv->lines);
    }
    if (sv->is_dimmed) free(sv->is_dimmed);
    if (sv->mapped_lines) free(sv->mapped_lines);
    sv->lines = NULL;
    sv->is_dimmed = NULL;
    sv->mapped_lines = NULL;
    sv->count = 0;
}

void source_view_render(SourceView *sv, CommonState *s, Env env, int current_line, Rectangle bounds) {
    (void)env;
    if (sv->count == 0) return;

    int highlight_line = current_line;
    if (current_line > 0 && current_line <= (int)sv->count) {
        highlight_line = sv->mapped_lines[current_line - 1] + 1;
    }

    float font_size = 20.0f;
    float line_height = font_size * 1.2f;
    
    // Auto-scroll logic: keep highlight_line centered if possible
    float visible_lines = bounds.height / line_height;
    int start_line = highlight_line - visible_lines / 2;
    if (start_line < 0) start_line = 0;
    if (start_line > (int)sv->count - (int)visible_lines) start_line = (int)sv->count - (int)visible_lines;
    if (start_line < 0) start_line = 0; // if content is smaller than view

    int end_line = start_line + (int)visible_lines + 1;
    if (end_line > (int)sv->count) end_line = (int)sv->count;

    BeginScissorMode(bounds.x, bounds.y, bounds.width, bounds.height);
    
    // Draw background for source view
    DrawRectangleRec(bounds, (Color){ 30, 30, 30, 255 });

    float y = bounds.y;
    for (int i = start_line; i < end_line; ++i) {
        bool is_current = (i + 1 == highlight_line); // line numbers are 1-based usually
        
        if (is_current) {
            DrawRectangle(bounds.x, y, bounds.width, line_height, (Color){ 60, 60, 50, 255 });
        }

        char buf[16];
        snprintf(buf, sizeof(buf), "%3d", i + 1);
        DrawTextEx(s->font, buf, (Vector2){bounds.x + 5, y}, font_size, 1, GRAY);
        
        Color text_color = is_current ? WHITE : (sv->is_dimmed[i] ? DARKGRAY : LIGHTGRAY);
        DrawTextEx(s->font, sv->lines[i], (Vector2){bounds.x + 50, y}, font_size, 1, text_color);
        y += line_height;
    }
    EndScissorMode();
}

#endif // COMMON_IMPLEMENTATION

#endif // COMMON_H_
