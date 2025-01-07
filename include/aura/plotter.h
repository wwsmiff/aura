#ifndef AURA_PLOTTER_H_
#define AURA_PLOTTER_H_

#include <cairo/cairo.h>

typedef struct {
  cairo_t *cairo_context;
  cairo_surface_t *cairo_surface;
  float width, height;
} aura_Plotter_t;

aura_Plotter_t *aura_plotter_create(float width, float height);
void aura_plotter_plot(aura_Plotter_t *plotter);
void aura_plotter_export_to_png(aura_Plotter_t *plotter, const char *path);
void aura_plotter_destroy(aura_Plotter_t *plotter);

#endif // AURA_PLOTTER_H_
