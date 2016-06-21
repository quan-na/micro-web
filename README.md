> **Project status**: *initialized* - It is not ready to be seen.

Generic Ideas
=============

MicroWeb project is intended to create web servers that run on as small as possible programs. The main micro-web server will receive requests from client, pass them through IPC to other servers. All servers will be written in C (not C++) and follow some communication rules that allow the web application to be started like this:
> image-server -u "/image" | js-server -u "/js" | bussiness-server | micro-server -p 8080

License
=======

This software use source code of following projects:
 - [Mongoose](http://cesanta.com/mongoose.shtml) : Embeded web server for C/C++
 - [Protothreads](http://dunkels.com/adam/pt/index.html) : Extremely lightweight stackless threads designed for severely memory constrained systems.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
