#include "aura/plotter.h"
#include "aura/string.h"
#include <cairo/cairo.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define AURA_STATE_OUTLINE_RADIUS 50.0f
#define AURA_TEXT_SIZE 24.0f

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

void aura_plotter_draw_state(aura_Plotter_t *plotter, float cx, float cy,
                             aura_String_t label) {
  char label_data[label.len + 1];
  for (size_t i = 0; i < label.len; ++i) {
    label_data[i] = label.data[i];
  }
  label_data[label.len] = '\0';

  cairo_new_path(plotter->cairo_context);
  cairo_set_source_rgb(plotter->cairo_context, 0.3f, 0.3f, 0.3f);
  cairo_arc(plotter->cairo_context, cx, cy, AURA_STATE_OUTLINE_RADIUS, 0.0f,
            2 * M_PI);
  cairo_stroke(plotter->cairo_context);
  cairo_text_extents_t extents;
  cairo_text_extents(plotter->cairo_context, label_data, &extents);
  cairo_move_to(plotter->cairo_context, cx - (extents.width / 2),
                cy + (extents.height / 2));
  cairo_show_text(plotter->cairo_context, label_data);
}

void aura_plotter_plot(aura_Plotter_t *plotter) {
  cairo_set_font_size(plotter->cairo_context, AURA_TEXT_SIZE);
  cairo_new_path(plotter->cairo_context);
  cairo_set_source_rgb(plotter->cairo_context, 1.0f, 1.0f, 1.0f);
  cairo_rectangle(plotter->cairo_context, 0.0f, 0.0f, plotter->width,
                  plotter->height);
  cairo_fill(plotter->cairo_context);

  aura_plotter_draw_state(plotter, 150.0f, 150.0f, AURA_STRING_LITERAL("q0"));
}

void aura_plotter_export_to_png(aura_Plotter_t *plotter, const char *path) {
  cairo_surface_write_to_png(plotter->cairo_surface, path);
}

void aura_plotter_destroy(aura_Plotter_t *plotter) {
  cairo_surface_destroy(plotter->cairo_surface);
  cairo_destroy(plotter->cairo_context);
  free(plotter);
}
