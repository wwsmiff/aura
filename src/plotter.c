#include "aura/plotter.h"
#include "aura/state.h"
#include "aura/string.h"
#include <cairo/cairo.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define AURA_STATE_OUTLINE_RADIUS 35.0f
#define AURA_INNER_RADIUS_COEFFICIENT 1.2f
#define AURA_STATE_INNER_RADIUS                                                \
  (AURA_STATE_OUTLINE_RADIUS / AURA_INNER_RADIUS_COEFFICIENT)
#define AURA_FONT_SIZE 20.0f
#define AURA_FONT_FACE "Noto Sans Mono"
#define AURA_PADDING 15.0f
#define AURA_ARROW_HEAD_ANGLE (M_PI / 4.0f)
#define AURA_ARROW_HEAD_LENGTH 10.0f
#define AURA_SPRING_CONST 0.0005f
#define AURA_PHYSICS_STEPS 200

float aura_vec2f_magnitude(aura_Vec2f_t vec) {
  return sqrt((vec.x * vec.x) + (vec.y * vec.y));
}

aura_Vec2f_t aura_vec2f_set_magnitude(aura_Vec2f_t vec, float magnitude) {
  float original_magnitude = aura_vec2f_magnitude(vec);
  return (aura_Vec2f_t){.x = vec.x * magnitude / original_magnitude,
                        .y = vec.y * magnitude / original_magnitude};
}

aura_Vec2f_t aura_vec2f_add(aura_Vec2f_t v1, aura_Vec2f_t v2) {
  return (aura_Vec2f_t){.x = v1.x + v2.x, .y = v1.y + v2.y};
}

aura_Vec2f_t aura_vec2f_sub(aura_Vec2f_t v1, aura_Vec2f_t v2) {
  return (aura_Vec2f_t){.x = v1.x - v2.x, .y = v1.y - v2.y};
}

aura_Vec2f_t aura_vec2f_mul(aura_Vec2f_t v1, aura_Vec2f_t v2) {
  return (aura_Vec2f_t){.x = v1.x * v2.x, .y = v1.y * v2.y};
}

aura_Vec2f_t aura_vec2f_div(aura_Vec2f_t v1, aura_Vec2f_t v2) {
  return (aura_Vec2f_t){.x = v1.x / v2.x, .y = v1.y / v2.y};
}

aura_Vec2f_t aura_vec2f_scale(aura_Vec2f_t v, float scalar) {
  return (aura_Vec2f_t){.x = v.x * scalar, .y = v.y * scalar};
}

aura_Plotter_t *aura_plotter_create(float width, float height) {
  aura_Plotter_t *plotter = (aura_Plotter_t *)(malloc(sizeof(aura_Plotter_t)));
  plotter->width = width;
  plotter->height = height;

  plotter->cairo_surface =
      cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  plotter->cairo_context = cairo_create(plotter->cairo_surface);

  cairo_select_font_face(plotter->cairo_context, AURA_FONT_FACE,
                         CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);

  cairo_set_font_size(plotter->cairo_context, AURA_FONT_SIZE);

  plotter->nodes = NULL;
  plotter->states = NULL;
  plotter->num_nodes = 0;
  plotter->springs = NULL;
  plotter->num_springs = 0;

  return plotter;
}

void aura_plotter_draw_path(aura_Plotter_t *plotter, aura_Path_Info_t info) {
  char label_data[info.label.len + 1];
  for (size_t i = 0; i < info.label.len; ++i) {
    label_data[i] = info.label.data[i];
  }
  label_data[info.label.len] = '\0';
  cairo_new_path(plotter->cairo_context);
  cairo_move_to(plotter->cairo_context, info.start.x, info.start.y);
  cairo_line_to(plotter->cairo_context, info.end.x, info.end.y);
  cairo_stroke(plotter->cairo_context);
  float theta = atan2f(info.end.y - info.start.y, info.end.x - info.start.x);
  cairo_move_to(plotter->cairo_context, info.end.x, info.end.y);
  cairo_line_to(plotter->cairo_context,
                info.end.x - AURA_ARROW_HEAD_LENGTH *
                                 cosf(theta - AURA_ARROW_HEAD_ANGLE),
                info.end.y - AURA_ARROW_HEAD_LENGTH *
                                 sinf(theta - AURA_ARROW_HEAD_ANGLE));
  cairo_move_to(plotter->cairo_context, info.end.x, info.end.y);
  cairo_line_to(plotter->cairo_context,
                info.end.x - AURA_ARROW_HEAD_LENGTH *
                                 cosf(theta + AURA_ARROW_HEAD_ANGLE),
                info.end.y - AURA_ARROW_HEAD_LENGTH *
                                 sinf(theta + AURA_ARROW_HEAD_ANGLE));
  cairo_line_to(plotter->cairo_context,
                info.end.x - AURA_ARROW_HEAD_LENGTH *
                                 cosf(theta - AURA_ARROW_HEAD_ANGLE),
                info.end.y - AURA_ARROW_HEAD_LENGTH *
                                 sinf(theta - AURA_ARROW_HEAD_ANGLE));
  cairo_fill(plotter->cairo_context);

  if (info.label.len != 0) {

    cairo_text_extents_t extents;
    cairo_text_extents(plotter->cairo_context, label_data, &extents);
    cairo_move_to(plotter->cairo_context,
                  info.start.x + ((info.end.x - info.start.x) / 2),
                  info.start.y - 12.0f + ((info.end.y - info.start.y) / 2));
    cairo_show_text(plotter->cairo_context, label_data);
  }
}

void aura_plotter_draw_state(aura_Plotter_t *plotter, aura_State_Info_t info) {
  char label_data[info.label.len + 1];
  for (size_t i = 0; i < info.label.len; ++i) {
    label_data[i] = info.label.data[i];
  }
  label_data[info.label.len] = '\0';

  cairo_new_path(plotter->cairo_context);
  cairo_set_source_rgb(plotter->cairo_context, 0.3f, 0.3f, 0.3f);
  cairo_arc(plotter->cairo_context, info.pos.x, info.pos.y,
            AURA_STATE_OUTLINE_RADIUS, 0.0f, 2 * M_PI);
  cairo_stroke(plotter->cairo_context);

  if (info.type & AURA_STATE_FINAL) {
    cairo_new_path(plotter->cairo_context);
    cairo_arc(plotter->cairo_context, info.pos.x, info.pos.y,
              AURA_STATE_INNER_RADIUS, 0.0f, 2 * M_PI);
    cairo_stroke(plotter->cairo_context);
  }
  if (info.type & AURA_STATE_INITIAL) {
    cairo_new_path(plotter->cairo_context);
    aura_plotter_draw_path(
        plotter,
        (aura_Path_Info_t){
            {info.pos.x - AURA_STATE_OUTLINE_RADIUS - 50.0f, info.pos.y},
            {info.pos.x - AURA_STATE_OUTLINE_RADIUS, info.pos.y},
            AURA_STRING_LITERAL("")});
  }

  cairo_text_extents_t extents;
  cairo_text_extents(plotter->cairo_context, label_data, &extents);
  cairo_move_to(plotter->cairo_context, info.pos.x - (extents.width / 2),
                info.pos.y + (extents.height / 4));
  cairo_show_text(plotter->cairo_context, label_data);
  return;
}

void aura_plotter_init_physics_system(aura_Plotter_t *plotter,
                                      aura_Machine_t *machine) {
  srand(time(NULL));
  if (machine->type == AURA_MACHINE_DFA) {
    aura_DFA_Machine_t *dfa_machine = machine->variant.dfa;

    // In order to ensure changing the order of initializing wont cause weird
    // issues.
    plotter->num_nodes = dfa_machine->active_states;
    plotter->num_springs = (plotter->num_nodes * (plotter->num_nodes - 1)) / 2;

    plotter->nodes = (aura_Spring_Node_t *)(malloc(sizeof(aura_Spring_Node_t) *
                                                   plotter->num_nodes));

    plotter->springs =
        (aura_Spring_t *)(malloc(sizeof(aura_Spring_t) * plotter->num_springs));

    for (size_t i = 0; i < plotter->num_nodes; ++i) {
      plotter->nodes[i].position =
          (aura_Vec2f_t){.x = (rand() % (int)(plotter->width - 100)) + 100,
                         .y = (rand() % (int)(plotter->height - 100)) + 100};
      plotter->nodes[i].velocity = (aura_Vec2f_t){0, 0};
      plotter->nodes[i].acceleration = (aura_Vec2f_t){0, 0};
    }

    size_t count = 0;

    for (size_t i = 0; i < plotter->num_nodes; ++i) {
      for (size_t j = i + 1; j < plotter->num_nodes; ++j) {
        plotter->springs[count].n1 = &plotter->nodes[i];
        plotter->springs[count].n2 = &plotter->nodes[j];

        plotter->springs[count].k = AURA_SPRING_CONST;
        plotter->springs[count].resting_length = 300.0f;

        count++;
      }
    }
  }
}

void aura_plotter_init_states(aura_Plotter_t *plotter,
                              aura_Machine_t *machine) {
  // In order to ensure changing the order of initializing wont cause weird
  // issues.
  if (machine->type == AURA_MACHINE_DFA) {
    aura_DFA_Machine_t *dfa_machine = machine->variant.dfa;
    plotter->num_nodes = dfa_machine->active_states;
    plotter->states = (aura_State_Info_t *)(malloc(sizeof(aura_State_Info_t) *
                                                   plotter->num_nodes));

    int count = 0;

    for (size_t i = 0; i < MAX_STATES; ++i) {
      if (dfa_machine->states[i] != NULL) {
        plotter->states[count++] =
            (aura_State_Info_t){.label = dfa_machine->states[i]->label,
                                .pos = (aura_Vec2f_t){0.0f, 0.0f},
                                .type = dfa_machine->states[i]->type};
      }
    }
  }
}

void aura_plotter_update_spring(aura_Spring_t *spring) {
  aura_Vec2f_t force =
      aura_vec2f_sub(spring->n2->position, spring->n1->position);
  float magnitude =
      spring->k * (aura_vec2f_magnitude(force) - spring->resting_length);

  force = aura_vec2f_set_magnitude(force, magnitude);

  spring->n1->acceleration = force;
  spring->n2->acceleration = aura_vec2f_scale(force, -1.0f);
}

void aura_plotter_move_spring(aura_Spring_t *spring) {
  spring->n1->position =
      aura_vec2f_add(spring->n1->position, spring->n1->velocity);
  spring->n2->position =
      aura_vec2f_add(spring->n2->position, spring->n2->velocity);

  spring->n1->velocity =
      aura_vec2f_add(spring->n1->velocity, spring->n1->acceleration);
  spring->n2->velocity =
      aura_vec2f_add(spring->n2->velocity, spring->n2->acceleration);

  if (aura_vec2f_magnitude(spring->n1->velocity) > 5.0f) {
    spring->n1->velocity = aura_vec2f_set_magnitude(spring->n1->velocity, 5.0f);
  }

  if (aura_vec2f_magnitude(spring->n2->velocity) > 5.0f) {
    spring->n2->velocity = aura_vec2f_set_magnitude(spring->n2->velocity, 5.0f);
  }

  spring->n1->velocity = aura_vec2f_scale(spring->n1->velocity, 0.90f);
  spring->n2->velocity = aura_vec2f_scale(spring->n2->velocity, 0.90f);
}

void aura_plotter_update_physics_system(aura_Plotter_t *plotter) {
  for (int i = 0; i < plotter->num_springs; ++i) {
    aura_plotter_update_spring(&(plotter->springs[i]));
    aura_plotter_move_spring(&(plotter->springs[i]));
  }
}

int aura_spring_node_comparator(const void *a, const void *b) {
  aura_Spring_Node_t n1 = *(const aura_Spring_Node_t *)(a);
  aura_Spring_Node_t n2 = *(const aura_Spring_Node_t *)(b);

  if (n1.position.x > n2.position.x) {
    return 1;
  } else if (n1.position.x < n2.position.x) {
    return -1;
  }
  return 0;
}

void aura_plotter_draw_spring_system(aura_Plotter_t *plotter) {

  qsort(plotter->nodes, plotter->num_nodes, sizeof(aura_Spring_Node_t),
        aura_spring_node_comparator);

  for (size_t i = 0; i < plotter->num_nodes; ++i) {
    aura_plotter_draw_state(plotter,
                            (aura_State_Info_t){plotter->states[i].label,
                                                plotter->nodes[i].position,
                                                plotter->states[i].type});
  }
}

void aura_plotter_plot(aura_Plotter_t *plotter, aura_Machine_t *machine) {
  cairo_set_font_size(plotter->cairo_context, AURA_FONT_SIZE);
  cairo_new_path(plotter->cairo_context);
  cairo_set_source_rgb(plotter->cairo_context, 1.0f, 1.0f, 1.0f);
  cairo_rectangle(plotter->cairo_context, 0.0f, 0.0f, plotter->width,
                  plotter->height);
  cairo_fill(plotter->cairo_context);

  aura_plotter_init_physics_system(plotter, machine);
  aura_plotter_init_states(plotter, machine);
  for (size_t i = 0; i < AURA_PHYSICS_STEPS; ++i) {
    aura_plotter_update_physics_system(plotter);
  }
  aura_plotter_draw_spring_system(plotter);
}

void aura_plotter_export_to_png(aura_Plotter_t *plotter, const char *path) {
  cairo_surface_write_to_png(plotter->cairo_surface, path);
}

void aura_plotter_destroy(aura_Plotter_t *plotter) {
  cairo_surface_destroy(plotter->cairo_surface);
  cairo_destroy(plotter->cairo_context);
  free(plotter->nodes);
  free(plotter->springs);
  free(plotter->states);
  free(plotter);
}
