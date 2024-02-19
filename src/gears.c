#include <GL/gl.h>
#include <fmath.h>
#include <math.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265
#endif

static GLfloat view_rotx = 20.0, view_roty = 30.0, view_rotz = 0.0;
static GLfloat angle = 0.0;

/*
 *
 *  Draw a gear wheel.  You'll probably want to call this function when
 *  building a display list fm_sinfce we do a lot of trig here.
 *
 *  Input:  inner_radius - radius of hole at center
 *          outer_radius - radius at center of teeth
 *          width - width of gear
 *          teeth - number of teeth
 *          tooth_depth - depth of tooth
 */
static void gear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width,
                 GLint teeth, GLfloat tooth_depth) {
  GLint i;
  GLfloat r0, r1, r2;
  GLfloat angle, da;
  GLfloat u, v, len;

  r0 = inner_radius;
  r1 = outer_radius - tooth_depth / 2.0;
  r2 = outer_radius + tooth_depth / 2.0;

  da = 2.0 * M_PI / teeth / 4.0;

  glShadeModel(GL_FLAT);
  glNormal3f(0.0, 0.0, 1.0);

  /* draw front face */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= teeth; i++) {
    angle = i * 2.0 * M_PI / teeth;

    GLfloat angle_cos = fm_cosf(angle);
    GLfloat angle_sin = fm_sinf(angle);

    glVertex3f(r0 * angle_cos, r0 * angle_sin, width * 0.5);
    glVertex3f(r1 * angle_cos, r1 * angle_sin, width * 0.5);
    if (i < teeth) {
      glVertex3f(r0 * angle_cos, r0 * angle_sin, width * 0.5);
      glVertex3f(r1 * fm_cosf(angle + 3 * da), r1 * fm_sinf(angle + 3 * da),
                 width * 0.5);
    }
  }
  glEnd();

  /* draw front sides of teeth */
  glBegin(GL_QUADS);
  da = 2.0 * M_PI / teeth / 4.0;
  for (i = 0; i < teeth; i++) {
    angle = i * 2.0 * M_PI / teeth;

    glVertex3f(r1 * fm_cosf(angle), r1 * fm_sinf(angle), width * 0.5);
    glVertex3f(r2 * fm_cosf(angle + da), r2 * fm_sinf(angle + da), width * 0.5);
    glVertex3f(r2 * fm_cosf(angle + 2 * da), r2 * fm_sinf(angle + 2 * da),
               width * 0.5);
    glVertex3f(r1 * fm_cosf(angle + 3 * da), r1 * fm_sinf(angle + 3 * da),
               width * 0.5);
  }
  glEnd();

  glNormal3f(0.0, 0.0, -1.0);

/* draw back face */
#if 0
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= teeth; i++) {
    angle = i * 2.0 * M_PI / teeth;
    glVertex3f(r1 * fm_cosf(angle), r1 * fm_sinf(angle), -width * 0.5);
    glVertex3f(r0 * fm_cosf(angle), r0 * fm_sinf(angle), -width * 0.5);
    if (i < teeth) {
      glVertex3f(r1 * fm_cosf(angle + 3 * da), r1 * fm_sinf(angle + 3 * da),
                 -width * 0.5);
      glVertex3f(r0 * fm_cosf(angle), r0 * fm_sinf(angle), -width * 0.5);
    }
  }
  glEnd();
#endif

  /* draw back sides of teeth */
  glBegin(GL_QUADS);
  da = 2.0 * M_PI / teeth / 4.0;
  for (i = 0; i < teeth; i++) {
    angle = i * 2.0 * M_PI / teeth;

    glVertex3f(r1 * fm_cosf(angle + 3 * da), r1 * fm_sinf(angle + 3 * da),
               -width * 0.5);
    glVertex3f(r2 * fm_cosf(angle + 2 * da), r2 * fm_sinf(angle + 2 * da),
               -width * 0.5);
    glVertex3f(r2 * fm_cosf(angle + da), r2 * fm_sinf(angle + da),
               -width * 0.5);
    glVertex3f(r1 * fm_cosf(angle), r1 * fm_sinf(angle), -width * 0.5);
  }
  glEnd();

  /* draw outward faces of teeth */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i < teeth; i++) {
    angle = i * 2.0 * M_PI / teeth;

    glVertex3f(r1 * fm_cosf(angle), r1 * fm_sinf(angle), width * 0.5);
    glVertex3f(r1 * fm_cosf(angle), r1 * fm_sinf(angle), -width * 0.5);
    u = r2 * fm_cosf(angle + da) - r1 * fm_cosf(angle);
    v = r2 * fm_sinf(angle + da) - r1 * fm_sinf(angle);
    len = sqrt(u * u + v * v);
    u /= len;
    v /= len;
    glNormal3f(v, -u, 0.0);
    glVertex3f(r2 * fm_cosf(angle + da), r2 * fm_sinf(angle + da), width * 0.5);
    glVertex3f(r2 * fm_cosf(angle + da), r2 * fm_sinf(angle + da),
               -width * 0.5);
    glNormal3f(cos(angle), fm_sinf(angle), 0.0);
    glVertex3f(r2 * fm_cosf(angle + 2 * da), r2 * fm_sinf(angle + 2 * da),
               width * 0.5);
    glVertex3f(r2 * fm_cosf(angle + 2 * da), r2 * fm_sinf(angle + 2 * da),
               -width * 0.5);
    u = r1 * fm_cosf(angle + 3 * da) - r2 * fm_cosf(angle + 2 * da);
    v = r1 * fm_sinf(angle + 3 * da) - r2 * fm_sinf(angle + 2 * da);
    glNormal3f(v, -u, 0.0);
    glVertex3f(r1 * fm_cosf(angle + 3 * da), r1 * fm_sinf(angle + 3 * da),
               width * 0.5);
    glVertex3f(r1 * fm_cosf(angle + 3 * da), r1 * fm_sinf(angle + 3 * da),
               -width * 0.5);
    glNormal3f(cos(angle), fm_sinf(angle), 0.0);
  }

  glVertex3f(r1 * fm_cosf(0), r1 * fm_sinf(0), width * 0.5);
  glVertex3f(r1 * fm_cosf(0), r1 * fm_sinf(0), -width * 0.5);

  glEnd();

  glShadeModel(GL_SMOOTH);

  /* draw inside radius cylinder */
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= teeth; i++) {
    angle = i * 2.0 * M_PI / teeth;

    GLfloat angle_cos = fm_cosf(angle);
    GLfloat angle_sin = fm_sinf(angle);

    glNormal3f(-angle_cos, -angle_sin, 0.0);
    glVertex3f(r0 * angle_cos, r0 * angle_sin, -width * 0.5);
    glVertex3f(r0 * angle_cos, r0 * angle_sin, width * 0.5);
  }
  glEnd();
}

void draw_gears(void) {
  static GLfloat red[4] = {0.8, 0.1, 0.0, 1.0};
  static GLfloat green[4] = {0.0, 0.8, 0.2, 1.0};
  static GLfloat blue[4] = {0.2, 0.2, 1.0, 1.0};

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glRotatef(view_rotx, 1.0, 0.0, 0.0);
  glRotatef(view_roty, 0.0, 1.0, 0.0);
  glRotatef(view_rotz, 0.0, 0.0, 1.0);

  glPushMatrix();
  glTranslatef(-3.0, -2.0, 0.0);
  glRotatef(angle, 0.0, 0.0, 1.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
  gear(1.0, 4.0, 1.0, 20, 0.7);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(3.1, -2.0, 0.0);
  glRotatef(-2.0 * angle - 9.0, 0.0, 0.0, 1.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green);
  gear(0.5, 2.0, 2.0, 10, 0.7);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-3.1, 4.2, 0.0);
  glRotatef(-2.0 * angle - 25.0, 0.0, 0.0, 1.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue);
  gear(1.3, 2.0, 0.5, 10, 0.7);
  glPopMatrix();

  angle += 1;

  glPopMatrix();
}

/* new window size or exposure */
void reshape(int width, int height) {
  glViewport(0, 0, (GLint)width, (GLint)height);
  GLfloat h = (GLfloat)height / (GLfloat)width;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -h, h, 5.0, 70.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -50.0);
}

void init_gears(void) {
  static GLfloat light_pos[4] = {5.0, 5.0, 10.0, 0.0};
  static GLfloat light_diff[4] = {1.0, 1.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
}
