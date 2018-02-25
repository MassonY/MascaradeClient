#include "MascaradeClient.h"
#include <iostream>
#include "Log.h"
#include "Data.h"
#include "Updater.h"
#include "Drawer.h"
#include "NotificationManager.h"
#include "MenuManager.h"

MascaradeClient::MascaradeClient()
{
	Drawer::I()->setWindow(&m_window);
	loadWindow();
	NotificationManager::I()->AddObserver("game_exit", this, &MascaradeClient::exit);
	Input::Manager::I()->addEventTrigger(sf::Event::Closed, "game_exit");
	MenuManager::I()->load();
}


MascaradeClient::~MascaradeClient()
{
}

void MascaradeClient::launch()
{
	m_connection.connect(Data::json().at("host"), Data::json().at("port"));
	while (m_window.isOpen()) {
		sf::Event e;
		while (m_window.pollEvent(e))
			Input::Manager::I()->handleEvent(e);
		
		Updater::I()->onUpdate();
		Drawer::I()->draw();
	}
	m_connection.disconnect();
}

void MascaradeClient::login()
{
	/*Log::debug() << "login begin";
	std::string s;
	std::cin >> s;
	m_packet.clear();
	m_packet << "login()" << "name:"+s;
	m_connection.send(m_packet);
	Log::debug() << "login end";*/
}

void MascaradeClient::loadWindow()
{
	int windowMode = Data::I()->data.at("FullScreen");
	int width = Data::json().at("WindowSize").at("x");
	int height = Data::json().at("WindowSize").at("y");
	std::string ApplicationName = Data::json().at("Application Name");
	if(windowMode == 0) // windowed
	{
		m_window.create(sf::VideoMode(width, height), ApplicationName);	
	}
	else if(windowMode == 1){ // fullscreen
		m_window.create(sf::VideoMode().getDesktopMode(), ApplicationName, sf::Style::Fullscreen);
	}
	else if (windowMode == 2) { // borderless
		m_window.create(sf::VideoMode(width, height), ApplicationName, sf::Style::None);
	}
	else{ 
		m_window.create(sf::VideoMode().getDesktopMode(), ApplicationName, sf::Style::None);
	}
	Drawer::I()->clear();
}

void MascaradeClient::exit()
{
	m_window.close();
}
