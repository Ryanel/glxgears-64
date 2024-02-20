#include <GL/gl.h>
#include <GL/gl_integration.h>
#include <GL/glu.h>
#include <libdragon.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <usb.h>

// Prototypes
void init_gears();
void draw_gears();
void reshape(int, int);

int main() {
  // Initialize the display
  display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE,
               ANTIALIAS_RESAMPLE_FETCH_ALWAYS);
  surface_t zbuffer =
      surface_alloc(FMT_RGBA16, display_get_width(), display_get_height());

  rdpq_init();
  gl_init();

  int gears_were_init = 0;

  while (1) {
    // Setup Display
    surface_t *disp = display_get();
    rdpq_attach(disp, &zbuffer);

    // Clear screen and init environment
    gl_context_begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_MULTISAMPLE_ARB);

    // Initialize gears, if they haven't been initialized yet
    if (gears_were_init == 0) {
      reshape(display_get_width(), display_get_height());
      init_gears();
      gears_were_init = 1;
    }

    // Draw gears using OpenGL
    draw_gears();

    // Submit frame
    gl_context_end();
    rdpq_detach_show();
  }
}