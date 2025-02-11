#ifndef AURA_PLOTTER_H_
#define AURA_PLOTTER_H_

#include "aura/machine.h"
#include <cairo/cairo.h>

typedef struct {
  float x, y;
} aura_Vec2f_t;

typedef struct {
  aura_Vec2f_t position;
  aura_Vec2f_t velocity;
  aura_Vec2f_t acceleration;
} aura_Spring_Node_t;

typedef struct {
  float k;
  float resting_length;
  aura_Spring_Node_t *n1, *n2;
} aura_Spring_t;

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

typedef struct {
  cairo_t *cairo_context;
  cairo_surface_t *cairo_surface;
  float width, height;

  aura_State_Info_t *states;
  aura_Spring_Node_t *nodes;
  size_t num_nodes;

  aura_Spring_t *springs;
  size_t num_springs;

} aura_Plotter_t;

aura_Plotter_t *aura_plotter_create(float width, float height);
void aura_plotter_plot(aura_Plotter_t *plotter, aura_Machine_t *machine);
void aura_plotter_export_to_png(aura_Plotter_t *plotter, const char *path);
void aura_plotter_destroy(aura_Plotter_t *plotter);

#endif // AURA_PLOTTER_H_
