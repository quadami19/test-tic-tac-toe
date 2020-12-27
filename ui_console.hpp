#pragma once
#include <iostream>
#include <cassert>
#include "game.hpp"

namespace ttt {
namespace ui {

struct Console
{
	private:
		game::Engine* m_game = nullptr;

	public:
		Console() {}

		void run()
		{
			auto fieldSize = getSize();
			auto players = getPlayers();
			char pl1Symbol;
			int pl1Index, pl2Index;

			game::Engine game(fieldSize, fieldSize);
			m_game = &game;

			if (players == 1) {
				pl1Symbol = getPlSymbol();
				pl1Index = m_game->symbolToNum(pl1Symbol);
				pl2Index = (pl1Index + 1) % 2;
			}

			switch (players) {
				case 0: m_game->setAiPlayer(0, true); m_game->setAiPlayer(1, true); break;
				case 1: m_game->setAiPlayer(pl1Index, false); m_game->setAiPlayer(pl2Index, true); break;
				case 2: m_game->setAiPlayer(0, false); m_game->setAiPlayer(1, false); break;
			}

			m_game->start();

			while (!m_game->isGameOver()) {
				if (m_game->isCurrPlayerAi()) aiTurn();
				else plTurn(m_game->currPlayerNum());
			}

			if (m_game->winner() != -1)
			{
				std::cout << "Winner player " << m_game->winner() + 1 << std::endl;
			}

			std::cout << "Game over." << std::endl;
			m_game = nullptr;
		}

	private:

		int getSize()
		{
			int ret;

			while (true)
			{
				std::cout << "Select size (3..5): ";
				std::cin >> ret;

				if (std::cin.fail())
				{
					std::string str;
					std::cin.clear();
					std::getline(std::cin, str);
					std::cout << "Invalid size " << str << "\n";
				}
				else if (ret > 5 || ret < 3)
				{
					std::cout << "Invalid size " << ret << "\n";
				}
				else {
					break;
				}
			}
			std::cout << "Size: " << ret << std::endl;

			return ret;
		}

		int getPlayers()
		{
			int ret;

			while (true)
			{
				std::cout << "Number of players (0..2): ";
				std::cin >> ret;

				if (std::cin.fail())
				{
					std::string str;
					std::cin.clear();
					std::getline(std::cin, str);
					std::cout << "Invalid input " << str << "\n";
				}
				else if (ret > 2 || ret < 0)
				{
					std::cout << "Invalid number " << ret << "\n";
				}
				else {
					break;
				}
			}
			std::cout << "Players: " << ret << std::endl;

			return ret;
		}

		char getPlSymbol()
		{
			char ret;

			while (true)
			{
				std::cout << "Select player 1 symbol ('" << (char)game::SYMBOL_O
				          << "' or '" << (char)game::SYMBOL_X << "): ";
				std::cin >> ret;

				if (ret == game::SYMBOL_O || ret == game::SYMBOL_X)
				{
					break;
				}
				else
				{
					std::cout << "Invalid side " << ret << "\n";
					std::string str;
					std::getline(std::cin, str);
				}
			}
			std::cout << "Selected " << ret << std::endl;

			return ret;
		}

		void plTurn(int _plNum)
		{
			std::cout << "Player " << _plNum + 1 << " turn. ";

			while (true)
			{
				Vector2D p;

				std::cout << "Place character (X, Y): ";
				std::cin >> p.x >> p.y;
				--p.x, --p.y;

				if (std::cin.fail())
				{
					std::string str;
					std::cin.clear();
					std::getline(std::cin, str);
					std::cout << "Invalid input " << str << "\n";
				}

				if (m_game->place(p))
				{
					m_game->turn();
					break;
				}
				else
				{
					std::cout << "Invalid coordinates entered.\n";
				}
			}
			printField();
		}

		void aiTurn()
		{
			std::cout << "AI's turn...\n";
			m_game->turn();
			printField();
		}

		void printField()
		{
			Vector2D p;

			std::cout << "    ";
			for(p.x = 0; p.x < m_game->field().size(); p.x++) std::cout << "  " << p.x + 1 << "  ";
			std::cout << "\n";

			for (p.y = 0; p.y < m_game->field().size(); p.y++)
			{
				std::cout << " " << p.y + 1 << " :";
				for (p.x = 0; p.x < m_game->field().size(); p.x++)
				{
					std::cout << " [" << m_game->field().cell(p) << "] ";
				}
				std::cout << "\n";
			}
		}
};

}
}
