/*
*   This project was first created to enable author's DB training together with Qt Framework
*  and C++. This project has begun without any knowledge of C++ programming and OO things.
*  The idea behind this project was to have a large file with an enourmeous amount of data to
*  realize data analysis onto one dataset to trace warm regions of a table where possible
*  concentrations of occurrences could be perceived in a range of time. (Or something like this)
*  The only one thing I thought to the time were the 'loterias da Caixa' that had all dataset
*  public and the result could bring a lot of fun at all. One important detail: all the data
*  were disposed inside an html file compressed in a zip file that needed to be downloaded from
*  the Caixa's servers...
*  All that I wanted a priori were satisfied... The history behind is more detailed into a
*  text file called History.md at the repository.
*
*  This is NOT A PROFESSIONAL PROJECT nor even made to be a PRODUCT.
*
*  All this software is free to use and modify according with all policies behind the Qt Frame-
*  work license. You can redistribute it and/or modify it under the terms of the GNU General
*  Public License as published by the Free Software Foundation, either version 3 of the License,
*  or (at your option) any later version.
*
*  This program is distributed to be useful and help to spread knowledge, but WITHOUT ANY WARRANTY;
*  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*  See the GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*  Author: Claudio Marques <manutronics@gmail.com>
*/

#include "colorslider.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ColorSlider w;
    w.show();

    return a.exec();
}
