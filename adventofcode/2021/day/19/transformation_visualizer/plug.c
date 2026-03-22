#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <raylib.h>
#include <raymath.h>

#include "nob.h"
#include "env.h"
#include "interpolators.h"
#include "tasks.h"
#include "plug.h"

#define PLUG(name, ret, ...) ret name(__VA_ARGS__);
LIST_OF_PLUGS
#undef PLUG

// --- DATA STRUCTURES ---

typedef struct {
    Vector3 origin;
    Vector3 i_hat;
    Vector3 j_hat;
    Vector3 k_hat;
    float opacity;
} ReferenceFrame;

typedef struct {
    size_t size;
    Arena arena_state;
    Arena arena_assets;
    bool finished;
    
    // Global Camera
    Camera3D camera;

    // Scene State
    int current_scene_index;
    Task task;

    // Visual Elements
    ReferenceFrame f0; // For TransformationVisualizer
    //ReferenceFrame f1; // For MatrixTransformScene
    //ReferenceFrame fa; // For CompareFramesScene
    //ReferenceFrame fb; // For CompareFramesScene

} Plug;

static Plug *p = NULL;

// --- HELPERS ---

static ReferenceFrame default_frame(Vector3 origin) {
    return (ReferenceFrame){
        .origin = origin,
        .i_hat = (Vector3){1.0f, 0.0f, 0.0f},
        .j_hat = (Vector3){0.0f, 1.0f, 0.0f},
        .k_hat = (Vector3){0.0f, 0.0f, 1.0f},
        .opacity = 0.0f // Start invisible
    };
}

static void draw_vector(Vector3 start, Vector3 vec, Color color, float opacity) {
    Vector3 end = Vector3Add(start, vec);
    Color c = ColorAlpha(color, opacity);
    DrawLine3D(start, end, c);
    DrawSphere(end, 0.05f, c); // Tip
}

static void draw_frame(ReferenceFrame *f) {
    if (f->opacity <= 0.01f) return;
    
    Color color_x = ColorAlpha(RED, f->opacity);
    Color color_y = ColorAlpha(GREEN, f->opacity);
    Color color_z = ColorAlpha(BLUE, f->opacity);
    Color color_origin = ColorAlpha(WHITE, f->opacity);

    draw_vector(f->origin, f->i_hat, color_x, f->opacity);
    draw_vector(f->origin, f->j_hat, color_y, f->opacity);
    draw_vector(f->origin, f->k_hat, color_z, f->opacity);
    DrawSphere(f->origin, 0.05f, color_origin);
}

// --- TASKS ---

static Task scene_transformation_visualizer(Arena *a) {
    ReferenceFrame *f0 = &p->f0;
    *f0 = default_frame((Vector3){0,0,0});

    // Rotation Matrix: 90 deg around Z
    /*
    Vector3 rot_i = (Vector3){0, 1, 0};
    Vector3 rot_j = (Vector3){-1, 0, 0};
    Vector3 rot_k = (Vector3){0, 0, 1};
    */
    Vector3 rot_i = (Vector3){1, 0, 0};
    Vector3 rot_j = (Vector3){0, 1, 0};
    Vector3 rot_k = (Vector3){0, 0, 1};

    Vector3 shift_val = (Vector3){2, 0, 0};
    Vector3 final_origin = Vector3Add(f0->origin, shift_val);

    return task_seq(a,
        task_move_scalar(a, &f0->opacity, 1.0f, 1.0f, FUNC_SMOOTHSTEP),
        task_wait(a, 0.5f),
        task_group(a,
            task_move_vec3(a, &f0->i_hat, rot_i, 1.0f, FUNC_SMOOTHSTEP),
            task_move_vec3(a, &f0->j_hat, rot_j, 1.0f, FUNC_SMOOTHSTEP),
            task_move_vec3(a, &f0->k_hat, rot_k, 1.0f, FUNC_SMOOTHSTEP)
        ),
        task_move_vec3(a, &f0->origin, final_origin, 1.0f, FUNC_SMOOTHSTEP),
        task_wait(a, 2.0f)
    );
}

/*
static Task scene_matrix_transform(Arena *a) {
    ReferenceFrame *f1 = &p->f1;
    *f1 = default_frame((Vector3){0,0,0});
    
    // Rotation Matrix: 90 deg around Z
    Vector3 target_i = (Vector3){0, 1, 0};
    Vector3 target_j = (Vector3){-1, 0, 0};
    Vector3 target_k = (Vector3){0, 0, 1};
    
    Vector3 translation = (Vector3){3, 2, 1};
    Vector3 final_origin = Vector3Add(f1->origin, translation);

    return task_seq(a,
        task_move_scalar(a, &f1->opacity, 1.0f, 1.0f, FUNC_SMOOTHSTEP),
        task_wait(a, 0.5f),
        task_group(a,
            task_move_vec3(a, &f1->i_hat, target_i, 2.0f, FUNC_SMOOTHSTEP),
            task_move_vec3(a, &f1->j_hat, target_j, 2.0f, FUNC_SMOOTHSTEP),
            task_move_vec3(a, &f1->k_hat, target_k, 2.0f, FUNC_SMOOTHSTEP)
        ),
        task_wait(a, 0.5f),
        task_move_vec3(a, &f1->origin, final_origin, 2.0f, FUNC_SMOOTHSTEP),
        task_wait(a, 2.0f)
    );
}
*/

/*
static Task scene_compare_frames(Arena *a) {
    ReferenceFrame *fa = &p->fa;
    ReferenceFrame *fb = &p->fb;
    
    Vector3 pos_a = (Vector3){0,0,0};
    Vector3 pos_b = (Vector3){2,1,0};
    
    *fa = default_frame(pos_a);
    *fb = default_frame(pos_b);
    
    // Rotation Matrix: 90 deg around Z
    Vector3 target_i = (Vector3){0, 1, 0};
    Vector3 target_j = (Vector3){-1, 0, 0};
    Vector3 target_k = (Vector3){0, 0, 1};
    
    fb->i_hat = target_i;
    fb->j_hat = target_j;
    fb->k_hat = target_k;
    
    return task_seq(a,
        task_move_scalar(a, &fa->opacity, 1.0f, 1.0f, FUNC_SMOOTHSTEP),
        task_wait(a, 0.5f),
        task_move_scalar(a, &fb->opacity, 0.3f, 1.0f, FUNC_SMOOTHSTEP),
        task_wait(a, 0.5f),
        task_group(a,
            task_move_vec3(a, &fa->origin, pos_b, 3.0f, FUNC_SMOOTHSTEP),
            task_move_vec3(a, &fa->i_hat, target_i, 3.0f, FUNC_SMOOTHSTEP),
            task_move_vec3(a, &fa->j_hat, target_j, 3.0f, FUNC_SMOOTHSTEP),
            task_move_vec3(a, &fa->k_hat, target_k, 3.0f, FUNC_SMOOTHSTEP)
        ),
        task_wait(a, 2.0f)
    );
}
*/

static Task master_sequence(Arena *a) {
    return task_seq(a,
        scene_transformation_visualizer(a),
        task_move_scalar(a, &p->f0.opacity, 0.0f, 0.5f, FUNC_SMOOTHSTEP)/*,
        scene_matrix_transform(a),
        task_move_scalar(a, &p->f1.opacity, 0.0f, 0.5f, FUNC_SMOOTHSTEP),
        scene_compare_frames(a),
        task_move_scalar(a, &p->fa.opacity, 0.0f, 0.5f, FUNC_SMOOTHSTEP),
        task_move_scalar(a, &p->fb.opacity, 0.0f, 0.5f, FUNC_SMOOTHSTEP)*/
    );
}

// --- PLUG IMPL ---

static void load_assets(void)
{
    arena_reset(&p->arena_assets);
    task_vtable_rebuild(&p->arena_assets);
    TraceLog(LOG_INFO, "Transformation Visualizer: Assets and VTable loaded");
}

void plug_init(void)
{
    p = malloc(sizeof(*p));
    assert(p != NULL);
    memset(p, 0, sizeof(*p));
    p->size = sizeof(*p);
    
    p->camera.position = (Vector3){ 5.0f, 5.0f, 5.0f };
    p->camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    p->camera.up = (Vector3){ 0.0f, 0.0f, 1.0f };
    p->camera.fovy = 45.0f;
    p->camera.projection = CAMERA_PERSPECTIVE;

    load_assets();
    plug_reset();
}

void *plug_pre_reload(void)
{
    return p;
}

void plug_post_reload(void *state)
{
    p = state;
    if (p->size < sizeof(*p)) {
        p = realloc(p, sizeof(*p));
        p->size = sizeof(*p);
    }
    load_assets();
}

void plug_update(Env env)
{
    p->finished = task_update(p->task, env);
    ClearBackground(BLACK);
    BeginMode3D(p->camera);
    DrawGrid(10, 1.0f);
    draw_frame(&p->f0);
    //draw_frame(&p->f1);
    //draw_frame(&p->fa);
    //draw_frame(&p->fb);
    EndMode3D();
}

void plug_reset(void)
{
    arena_reset(&p->arena_state);
    p->f0 = default_frame((Vector3){0,0,0});
    //p->f1 = default_frame((Vector3){0,0,0});
    //p->fa = default_frame((Vector3){0,0,0});
    //p->fb = default_frame((Vector3){0,0,0});
    p->task = master_sequence(&p->arena_state);
    p->finished = false;
}

bool plug_finished(void)
{
    return p->finished;
}

#define ARENA_IMPLEMENTATION
#include "arena.h"
#include "tasks.c"
