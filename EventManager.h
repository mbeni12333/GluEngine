#pragma once
#include<SFML/Graphics.hpp>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>
enum class EventType{
	KeyDown = sf::Event::KeyPressed,
	KeyUP = sf::Event::KeyReleased,
	MButtonDown = sf::Event::MouseButtonPressed,
	MButtonUp = sf::Event::MouseButtonReleased,
	MouseWheel = sf::Event::MouseWheelMoved,
	WindowResized = sf::Event::Resized,
	GainedFocus = sf::Event::GainedFocus,
	LostFocus = sf::Event::LostFocus,
	MouseEntered = sf::Event::MouseEntered,
	MouseLeft = sf::Event::MouseLeft,
	Closed = sf::Event::Closed,
	TextEntered = sf::Event::TextEntered,
	Keyboard = sf::Event::Count + 1,
	Mouse,
	Joystick
};

struct EventInfo{
	EventInfo(){
		m_code = 0;
	}
	EventInfo(int l_event){
		m_code = l_event;
	}
	// use one of the stuff
	union{
		int m_code;
	};
};
using Events = std::vector<std::pair<EventType, EventInfo>>;

struct EventDetails{
	EventDetails(const std::string& l_bindname): m_name(l_bindname){
		Clear();
	}
	std::string m_name;
	sf::Vector2i m_size;
	sf::Uint32 m_textEntered;
	sf::Vector2i m_mouse;
	int m_mouseWheelDelta;
	int m_keyCode;

	void Clear(){
		m_size = sf::Vector2i(0, 0);
		m_textEntered = 0;
		m_mouse = sf::Vector2i(0, 0);
		m_mouseWheelDelta = 0;
		m_keyCode = -1;
	}
};

struct Binding{
	Binding(const std::string& l_name): m_name(l_name), m_details(l_name), c(0){

	}
	void BindEvent(EventType l_type, EventInfo l_info = EventInfo()){
		m_events.emplace_back(l_type, l_info);
	}
	Events m_events;
	EventDetails m_details;
	std::string m_name;
	int c;
	
};

using Bindings = std::unordered_map<std::string, Binding*>;
using CallBacks = std::unordered_map<std::string, std::function<void(EventDetails*)>>;

class EventManager{
public:
	EventManager();
	~EventManager();

	bool AddBinding(Binding* l_binding);
	bool RemoveBinding(std::string l_name);

	void setFocus(const bool& l_focus);

	 
	template<class T>
	bool AddCallBack(const std::string& l_name,
					 void(T::*l_func)(EventDetails*), T* l_instance){

		auto temp = std::bind(l_func, l_instance, std::placeholders::_1);
		return m_callbacks.emplace(l_name, temp).second;
	}
	void RemoveCallBack(const std::string& l_name){
		m_callbacks.erase(l_name);
	}

	void HandleEvent(sf::Event& l_event);
	void Update();

	sf::Vector2i GetMousePos(sf::RenderWindow* l_wind = nullptr){
		return (l_wind ? sf::Mouse::getPosition(*l_wind) : sf::Mouse::getPosition());
	}

private:
	void LoadBindings();
	Bindings m_bindings;
	CallBacks m_callbacks;
	bool m_hasFocus;
};

