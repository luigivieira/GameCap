/* Copyright (C) 2016 Luiz Carlos Vieira (http://www.luiz.vieira.nom.br)
 *
 * This file is part of GameCap.
 *
 * GameCap is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GameCap is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
 
#include "application.h"
#include "window.h"
#include <QScreen>

using namespace gc;

int main(int argc, char** argv)
{
    Application oApp(argc, argv);

    Window oMain;
	oMain.show();
	oMain.resize(QApplication::primaryScreen()->size());
	oMain.move(0, 0);

	oApp.setMainWindow(&oMain);
    
	return oApp.exec();
}