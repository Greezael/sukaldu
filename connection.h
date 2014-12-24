/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

static bool loadScript(const char *name, bool line_by_line = false);

/*
    This file defines a helper function to open a connection to an
    in-memory SQLITE database and to create a test table.

    If you want to use another database, simply modify the code
    below. All the examples in this directory use this function to
    connect to a database.
*/
//! [0]
static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("Unable to establish a database connection.\n"
                     "This example needs SQLite support. Please read "
                     "the Qt SQL driver documentation for information how "
                     "to build it.\n\n"
                     "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

    loadScript("data/database_definition.sql");
    loadScript("data/database_sample.sql", true);

    return true;
}
//! [0]

static bool loadScript(const char * name, bool line_by_line)
{
    QSqlQuery query;
    std::ifstream inputFile(name);
    std::stringstream queryStr;
    std::string tmpStr;
    if(!inputFile)
        //Test file open
    {
        std::cerr << "Error opening input file: " << name << std::endl;
        std::exit(-1);
//        return -1; // Shouldn't arrive here
    }

    while (!inputFile.eof())
    {

        if (std::getline(inputFile, tmpStr) &&
                tmpStr.length() != 0 &&
                tmpStr.compare(0, 2, "--") != 0) {
            queryStr << tmpStr << std::endl;
        }
        if ((queryStr.str().length() != 0) &&
                (tmpStr.length() == 0 ||
                 inputFile.eof() ||
                 line_by_line))
        {
            query.exec(queryStr.str().c_str());
//            std::cout << "EXECUTE QUERY: " << std::endl;
//            std::cout << queryStr.str() << std::endl;
//            std::cout << "--------" << std::endl;
            if (query.lastError().type() != QSqlError::NoError)
            {
                std::cerr << "ERROR: " << query.lastError().text().toUtf8().constData() << std::endl;
                std::cerr << "IN QUERY: " << std::endl;
                std::cerr << queryStr.str() << std::endl;
                std::cerr << "--------" << std::endl;
                std::exit(-1);
            }
            queryStr.str(std::string());
        }
    }

    return true;
}

#endif
