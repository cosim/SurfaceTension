#include "glrender.h"
#include "coresystems.h"
#include "systemmanager.h"
#include <OpenGL/gl.h>

struct render_subsystem
{
	render_system* system;
	subsystem_render render_func;
	render_subsystem* next;
	
};

struct gl_render
{
	render_subsystem* subsystem_list;
	render_subsystem** subsystem_next;
};

void* init_gl_render_system(SystemManager* SM)
{
	gl_render* renderer = new gl_render();
	renderer->subsystem_list = NULL;
	renderer->subsystem_next = &renderer->subsystem_list;
	glut_system* glut = (glut_system*)system_manager_require(SM, "GLUT");
	glut_system_set_renderer(glut, renderer);

	glShadeModel(GL_SMOOTH);
	glClearColor(0.35f, 0.94f, 1.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	return renderer;
}

void destroy_gl_render_system(void* System)
{
	delete (gl_render*)System;
}

void gl_render_do_render(gl_render* renderer)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Camera System.
	//Vector DeltaCamera = MainPlayer->State.Location - CameraFocusLocation;
	//DeltaCamera *= DeltaCamera.Length()*CameraApproach/CameraWidth;
	//CameraFocusLocation += DeltaCamera;

	//float CameraHeight = CameraWidth*CameraAspect;
	//glOrtho(CameraFocusLocation.x-(CameraWidth/2.0f), CameraFocusLocation.x+(CameraWidth/2.0f), CameraFocusLocation.y-(CameraHeight/2.0f), CameraFocusLocation.y+(CameraHeight/2.0f), 0.0f, 2.0f);
	//glScalef(1.0f/CameraWidth, 1.0f/(CameraWidth*CameraAspect), 1.0);
	//gluOrtho2D(0, CameraWidth, CameraWidth*CameraAspect, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	render_subsystem* curr_render_system = renderer->subsystem_list;
	while(curr_render_system)
	{
		curr_render_system->render_func(renderer, curr_render_system->system);
		curr_render_system = curr_render_system->next;
	}

	// Do render of registered renderables.
	//RenderPlayer(MainPlayer);

	glColor3f(1.0f, 0.0f, 0.0f);
}

void declare_gl_render_system(SystemManager* SM)
{
	declare_system(SM, system_manager_symbol(SM, "glrender"), init_gl_render_system, destroy_gl_render_system);
}

void gl_render_register_render_system(gl_render* renderer, render_system* system, subsystem_render render_func)
{
	render_subsystem* new_link = new render_subsystem();
	new_link->system = system;
	new_link->render_func = render_func;
	*renderer->subsystem_next = new_link;
	renderer->subsystem_next = &new_link->next;
}
