#include "MyApp.h"
#include "SDL_GLDebugMessageCallback.h"

#include <imgui.h>

CMyApp::CMyApp()
{
}

CMyApp::~CMyApp()
{
}

void CMyApp::SetupDebugCallback()
{
	// engedélyezzük és állítsuk be a debug callback függvényt ha debug context-ben vagyunk 
	GLint context_flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
	if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
		glDebugMessageCallback(SDL_GLDebugMessageCallback, nullptr);
	}
}

bool CMyApp::Init()
{
	SetupDebugCallback();

	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	//
	// egyéb inicializálás
	//

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hátrafelé néző lapok eldobását
	glCullFace(GL_BACK);    // GL_BACK: a kamerától "elfelé" néző lapok, GL_FRONT: a kamera felé néző lapok

	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	return true;
}

void CMyApp::Clean()
{
}

void CMyApp::Update( const SUpdateInfo& updateInfo )
{
	
}

void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT)...
	// ... és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void CMyApp::RenderGUI()
{
	//ImGui::ShowDemoWindow();
	ImGui::Begin("MyApp");
	if (ImGui::ColorEdit3("Clear Color", value_ptr(_clearColor), ImGuiColorEditFlags_Float))
	{
		glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, 1.0f);
	}
	ImGui::End();
}

// https://wiki.libsdl.org/SDL2/SDL_KeyboardEvent
// https://wiki.libsdl.org/SDL2/SDL_Keysym
// https://wiki.libsdl.org/SDL2/SDL_Keycode
// https://wiki.libsdl.org/SDL2/SDL_Keymod

void CMyApp::KeyboardDown(const SDL_KeyboardEvent& key)
{
	switch (key.keysym.sym) {
	case SDLK_w:
		_keyEvents.insert('w');
		break;
	case SDLK_a:
		_keyEvents.insert('a');
		break;
	case SDLK_d:
		_keyEvents.insert('d');
		break;
	}
}

void CMyApp::KeyboardUp(const SDL_KeyboardEvent& key)
{
	switch (key.keysym.sym) {
	case SDLK_w:
		_keyEvents.erase('w');
		break;
	case SDLK_a:
		_keyEvents.erase('a');
		break;
	case SDLK_d:
		_keyEvents.erase('d');
		break;
	}
}

// https://wiki.libsdl.org/SDL2/SDL_MouseMotionEvent

void CMyApp::MouseMove(const SDL_MouseMotionEvent& mouse)
{

	if (_mouseEvents.find(1) != _mouseEvents.end()) {
		_clearColor.x = (((float)mouse.x) / ((float)_windowW));
	}
	if (_mouseEvents.find(2) != _mouseEvents.end()) {
		_clearColor.y = (((float)mouse.x) / ((float)_windowW));
	}
	if (_mouseEvents.find(3) != _mouseEvents.end()) {
		_clearColor.z = (((float)mouse.x) / ((float)_windowW));
	}

	if (_keyEvents.find('w') != _keyEvents.end()) {
		_clearColor.x = (((float)mouse.x) / ((float)_windowW));
	}
	if (_keyEvents.find('a') != _keyEvents.end()) {
		_clearColor.y = (((float)mouse.x) / ((float)_windowW));
	}
	if (_keyEvents.find('d') != _keyEvents.end()) {
		_clearColor.z = (((float)mouse.x) / ((float)_windowW));
	}

	glClearColor(
		_clearColor.x,
		_clearColor.y,
		_clearColor.z, 1.0f);
}

// https://wiki.libsdl.org/SDL2/SDL_MouseButtonEvent

void CMyApp::MouseDown(const SDL_MouseButtonEvent& mouse)
{
	switch (mouse.button) {
	case SDL_BUTTON_LEFT:
		_mouseEvents.insert(1);
		break;
	case SDL_BUTTON_MIDDLE:
		_mouseEvents.insert(2);
		break;
	case SDL_BUTTON_RIGHT:
		_mouseEvents.insert(3);
		break;
	}
}

void CMyApp::MouseUp(const SDL_MouseButtonEvent& mouse)
{
	switch (mouse.button) {
	case SDL_BUTTON_LEFT:
		_mouseEvents.erase(1);
		break;
	case SDL_BUTTON_MIDDLE:
		_mouseEvents.erase(2);
		break;
	case SDL_BUTTON_RIGHT:
		_mouseEvents.erase(3);
		break;
}
}

// https://wiki.libsdl.org/SDL2/SDL_MouseWheelEvent

void CMyApp::MouseWheel(const SDL_MouseWheelEvent& wheel)
{
}


// a két paraméterben az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	_windowW = _w;
	_windowH = _h;
	glViewport(0, 0, _w, _h);
}

