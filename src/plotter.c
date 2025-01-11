#include "aura/plotter.h"
#include "aura/state.h"
#include "aura/string.h"
#include <cairo/cairo.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define AURA_STATE_OUTLINE_RADIUS 35.0f
#define AURA_OUTER_RADIUS_COEFFICIENT 1.2f
#define AURA_STATE_OUTER_RADIUS                                                \
  (AURA_STATE_OUTLINE_RADIUS * AURA_OUTER_RADIUS_COEFFICIENT)
#define AURA_TEXT_SIZE 20.0f
#define AURA_PADDING 15.0f
#define AURA_ARROW_HEAD_ANGLE (M_PI / 4.0f)
#define AURA_ARROW_HEAD_LENGTH 10.0f

typedef struct {
  float x, y;
} aura_Vec2f_t;

typedef struct {
  aura_String_t label;
  aura_Vec2f_t pos;
  aura_StateType type;
} aura_State_Info_t;

typedef struct {
  aura_Vec2f_t start;
  aura_Vec2f_t end;
  aura_String_t label;
} aura_Path_Info_t;

aura_Plotter_t *aura_plotter_create(float width, float height) {
  aura_Plotter_t *plotter = (aura_Plotter_t *)(malloc(sizeof(aura_Plotter_t)));
  plotter->width = width;
  plotter->height = height;

  plotter->cairo_surface =
      cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  plotter->cairo_context = cairo_create(plotter->cairo_surface);

  printf("Created %.0fx%.0f plotter.\n", plotter->width, plotter->height);

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
              AURA_STATE_OUTER_RADIUS, 0.0f, 2 * M_PI);
    cairo_stroke(plotter->cairo_context);
  }
  if (info.type == (AURA_STATE_INITIAL | AURA_STATE_FINAL)) {
    cairo_new_path(plotter->cairo_context);
    aura_plotter_draw_path(
        plotter, (aura_Path_Info_t){
                     {info.pos.x - AURA_STATE_OUTER_RADIUS - 50.0f, info.pos.y},
                     {info.pos.x - AURA_STATE_OUTER_RADIUS, info.pos.y},
                     AURA_STRING_LITERAL("")});
  } else if (info.type == AURA_STATE_INITIAL) {
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
                info.pos.y + (extents.height / 2));
  cairo_show_text(plotter->cairo_context, label_data);
}

void aura_plotter_plot(aura_Plotter_t *plotter) {
  cairo_set_font_size(plotter->cairo_context, AURA_TEXT_SIZE);
  cairo_new_path(plotter->cairo_context);
  cairo_set_source_rgb(plotter->cairo_context, 1.0f, 1.0f, 1.0f);
  cairo_rectangle(plotter->cairo_context, 0.0f, 0.0f, plotter->width,
                  plotter->height);
  cairo_fill(plotter->cairo_context);

  aura_plotter_draw_state(
      plotter, (aura_State_Info_t){.label = AURA_STRING_LITERAL("q0"),
                                   .pos = {150.0f, 150.0f},
                                   .type = AURA_STATE_INITIAL});

  aura_plotter_draw_state(
      plotter,
      (aura_State_Info_t){.label = AURA_STRING_LITERAL("q1"),
                          .pos = {150.0f, 300.0f},
                          .type = AURA_STATE_INITIAL | AURA_STATE_FINAL});

  aura_plotter_draw_state(
      plotter, (aura_State_Info_t){.label = AURA_STRING_LITERAL("q2"),
                                   .pos = {150.0f, 400.0f},
                                   .type = AURA_STATE_FINAL});
}

void aura_plotter_export_to_png(aura_Plotter_t *plotter, const char *path) {
  cairo_surface_write_to_png(plotter->cairo_surface, path);
}

void aura_plotter_destroy(aura_Plotter_t *plotter) {
  cairo_surface_destroy(plotter->cairo_surface);
  cairo_destroy(plotter->cairo_context);
  free(plotter);
}
