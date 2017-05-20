/*
 * TestFile.cpp
 *
 *  Created on: May 18, 2016
 *      Author: j.zhou
 */
 
#include "TestFile.h"
#include "IoException.h"
#include "CMLogger.h"
#include <iostream>

using namespace cm;
using namespace std;

TEST_F(TestFile, OpenFileReadOnly) {
    string filename("/tmp/openFilerd.txt");
    remove(filename.c_str());    
    EXPECT_THROW(new File(filename, FILE_OPEN, FILE_READ_ONLY), IoException);

    cm::CMLogger::setLogLevel(cm::INFO);
 
    string filetext("123456789");
    string createFileCmd("echo \"" + filetext + "\" > " + filename);
    system(createFileCmd.c_str());
    File file(filename, FILE_OPEN, FILE_READ_ONLY);

    char buffer[32];
    int readBytes;
    int result = file.read(buffer, 32, readBytes);
    EXPECT_EQ(result, FILE_SUCC);
    int expectReadBytes = filetext.length() + 1;
    EXPECT_EQ(readBytes, expectReadBytes);  // filetext + LF (ascii: 0a)    
    cout << "read " << readBytes << " bytes: " << buffer << endl;

    // check the string TODO

    int writeBytes = 0;
    EXPECT_EQ(file.write(filetext.c_str(), filetext.length(), writeBytes), FILE_ERR);
}

TEST_F(TestFile, OpenFileWriteOnly) {
    string filename("/tmp/openFilewr.txt");
    remove(filename.c_str());    
    EXPECT_THROW(new File(filename, FILE_OPEN, FILE_WRITE_ONLY), IoException);
 
    // open exist file, write from the beginning
    string filetext("123456789");
    string createFileCmd("echo \"" + filetext + "\" > " + filename);
    system(createFileCmd.c_str());
    File file(filename, FILE_OPEN, FILE_WRITE_ONLY);
    char buffer[32];
    int readBytes = 0;
    string filetext2("abcdefghijklmnopqrst");
    int writeBytes = 0;
    int expectWriteBytes = filetext2.length();
    EXPECT_EQ(file.read(buffer, 32, readBytes), FILE_ERR);    
    EXPECT_EQ(file.write(filetext2.c_str(), filetext2.length(), writeBytes), FILE_SUCC);
    EXPECT_EQ(writeBytes, expectWriteBytes);

    File file2(filename, FILE_OPEN, FILE_READ_ONLY);
    int expectReadBytes = filetext2.length();
    EXPECT_EQ(file2.read(buffer, 32, readBytes), FILE_SUCC);  
    EXPECT_EQ(readBytes, expectReadBytes);  // filetext + LF (ascii: 0a)
    cout << "read " << readBytes << " bytes: " << buffer << endl;
    file.close();
    file2.close();
}

TEST_F(TestFile, OpenFileReadWrite) {
    string filename("/tmp/openFilerdwr.txt");
    remove(filename.c_str());    
    EXPECT_THROW(new File(filename, FILE_OPEN, FILE_READ_WRITE), IoException);
 
    string filetext("123456789");
    string createFileCmd("echo \"" + filetext + "\" > " + filename);
    system(createFileCmd.c_str());
    File file(filename, FILE_OPEN, FILE_READ_WRITE);

    // test read from beginning
    char buffer[32];
    int readBytes;
    int result = file.read(buffer, 32, readBytes);
    cout << "1 read " << readBytes << " bytes: " << buffer << endl;
    EXPECT_EQ(result, FILE_SUCC);
    int expectReadBytes = filetext.length() + 1;
    EXPECT_EQ(readBytes, expectReadBytes);  // filetext + LF (ascii: 0a)

    // check the string TODO

    // test write from current pos
    int writeBytes = 0;
    int expectWriteBytes = filetext.length();
    EXPECT_EQ(file.write(filetext.c_str(), filetext.length(), writeBytes), FILE_SUCC);
    EXPECT_EQ(writeBytes, expectWriteBytes);
    
    // test read from beginning again
    EXPECT_EQ(file.seek(0), 0L);
    result = file.read(buffer, 32, readBytes);
    expectReadBytes += filetext.length();
    cout << "2 read " << readBytes << " bytes: " << buffer << endl;
    EXPECT_EQ(result, FILE_SUCC);
    EXPECT_EQ(readBytes, expectReadBytes);
    file.close();

    // test write from the end of file
    File file2(filename, FILE_OPEN, FILE_READ_WRITE);
    string filetext2("abcdefghijklmnopqrst");
    expectWriteBytes = filetext2.length();
    EXPECT_EQ(file2.write(filetext2.c_str(), filetext2.length(), writeBytes), FILE_SUCC);
    EXPECT_EQ(writeBytes, expectWriteBytes);

    file2.close();
}

TEST_F(TestFile, OpenFileReadWriteAppend) {
    string filename("/tmp/openFilerdwrappend.txt");
    remove(filename.c_str());    
    EXPECT_THROW(new File(filename, FILE_OPEN, FILE_READ_WRITE), IoException);
 
    string filetext("123456789");
    string createFileCmd("echo \"" + filetext + "\" > " + filename);
    system(createFileCmd.c_str());
    File file(filename, FILE_OPEN, FILE_READ_WRITE_APPEND);

    // test read from beginning
    char buffer[32];
    int readBytes;
    int result = file.read(buffer, 32, readBytes);
    cout << "1 read " << readBytes << " bytes: " << buffer << endl;
    EXPECT_EQ(result, FILE_SUCC);
    int expectReadBytes = filetext.length() + 1;
    EXPECT_EQ(readBytes, expectReadBytes);  // filetext + LF (ascii: 0a)

    // check the string TODO

    // test write from current pos
    int writeBytes = 0;
    int expectWriteBytes = filetext.length();
    EXPECT_EQ(file.write(filetext.c_str(), filetext.length(), writeBytes), FILE_SUCC);
    EXPECT_EQ(writeBytes, expectWriteBytes);
    
    // test read from beginning again
    EXPECT_EQ(file.seek(0), 0L);
    result = file.read(buffer, 32, readBytes);
    expectReadBytes += filetext.length();
    cout << "2 read " << readBytes << " bytes: " << buffer << endl;
    EXPECT_EQ(result, FILE_SUCC);
    EXPECT_EQ(readBytes, expectReadBytes);
    file.close();

    // test write from the end of file
    File file2(filename, FILE_OPEN, FILE_READ_WRITE_APPEND);
    string filetext2("abcdefghijklmnopqrst");
    expectWriteBytes = filetext2.length();
    EXPECT_EQ(file2.write(filetext2.c_str(), filetext2.length(), writeBytes), FILE_SUCC);
    EXPECT_EQ(writeBytes, expectWriteBytes);

    file2.close();
}

TEST_F(TestFile, CreateFileReadOnly) {
    string filename("/tmp/createFilerd.txt");
    remove(filename.c_str());    

    File file(filename, FILE_CREATE, FILE_READ_ONLY);
 
    string filetext("123456789");   
    char buffer[32];
    int readBytes;
    int result = file.read(buffer, 32, readBytes);
    EXPECT_EQ(result, FILE_SUCC);
    int expectReadBytes = 0;
    EXPECT_EQ(readBytes, expectReadBytes);  // empty file

    // check the string TODO

    int writeBytes = 0;
    EXPECT_EQ(file.write(filetext.c_str(), filetext.length(), writeBytes), FILE_ERR);
    file.close();
    result = file.read(buffer, 32, readBytes);
    EXPECT_EQ(result, FILE_ERR);

    // create an existing file, truncate it
    string createFileCmd("echo \"" + filetext + "\" > " + filename);
    system(createFileCmd.c_str());
    File file2(filename, FILE_CREATE, FILE_READ_ONLY);
    result = file2.read(buffer, 32, readBytes);
    EXPECT_EQ(result, FILE_SUCC);
    expectReadBytes = 0;
    EXPECT_EQ(readBytes, expectReadBytes);  // empty file
}

TEST_F(TestFile, CreateFileWriteOnly) {
    string filename("/tmp/createFilewr.txt");
    remove(filename.c_str());    

    File file(filename, FILE_CREATE, FILE_WRITE_ONLY);
 
    string filetext("123456789");
    char buffer[32];
    int readBytes;
    EXPECT_EQ(file.read(buffer, 31, readBytes), FILE_ERR);

    int writeBytes = 0;
    EXPECT_EQ(file.write(filetext.c_str(), filetext.length(), writeBytes), FILE_SUCC);
    int expectWriteBytes = filetext.length();
    EXPECT_EQ(writeBytes, expectWriteBytes);

    file.close();
    EXPECT_EQ(file.write(filetext.c_str(), filetext.length(), writeBytes), FILE_ERR);

    // create an existing file, the file content will be truncated to become empty
    string filetext2("abcdefghijk");
    expectWriteBytes = filetext2.length();
    File file2(filename, FILE_CREATE, FILE_WRITE_ONLY);
    EXPECT_EQ(file2.write(filetext2.c_str(), filetext2.length(), writeBytes), FILE_SUCC);
    EXPECT_EQ(writeBytes, expectWriteBytes);
    file2.close();

    File file3(filename, FILE_OPEN, FILE_READ_ONLY);
    EXPECT_EQ(file3.read(buffer, 31, readBytes), FILE_SUCC);
    EXPECT_EQ(readBytes, expectWriteBytes);
}

TEST_F(TestFile, CreateFileReadWrite) {
    string filename("/tmp/createFilerdwr.txt");
    remove(filename.c_str());    

    File file(filename, FILE_CREATE, FILE_READ_WRITE);
 
    string filetext("123456789");
    char buffer[32];
    int readBytes;
    // int expectReadBytes = 0;
    EXPECT_EQ(file.read(buffer, 31, readBytes), FILE_SUCC);
    EXPECT_EQ(readBytes, 0);

    int writeBytes = 0;
    EXPECT_EQ(file.write(filetext.c_str(), filetext.length(), writeBytes), FILE_SUCC);
    int expectWriteBytes = filetext.length();
    EXPECT_EQ(writeBytes, expectWriteBytes);

    EXPECT_EQ(file.read(buffer, 31, readBytes), FILE_SUCC);
    EXPECT_EQ(readBytes, 0);
    file.seek(0);
    EXPECT_EQ(file.read(buffer, 31, readBytes), FILE_SUCC);
    EXPECT_EQ(readBytes, expectWriteBytes);

    file.close();
    EXPECT_EQ(file.write(filetext.c_str(), filetext.length(), writeBytes), FILE_ERR);
    EXPECT_EQ(file.read(buffer, 31, readBytes), FILE_ERR);

    // create an existing file, truncate it
    string filetext2("abcdefghijk");
    expectWriteBytes = filetext2.length();
    File file2(filename, FILE_CREATE, FILE_READ_WRITE);
    EXPECT_EQ(file2.write(filetext2.c_str(), filetext2.length(), writeBytes), FILE_SUCC);
    EXPECT_EQ(writeBytes, expectWriteBytes);
    file2.close();
    
    File file3(filename, FILE_OPEN, FILE_READ_ONLY);
    EXPECT_EQ(file3.read(buffer, 31, readBytes), FILE_SUCC);
    EXPECT_EQ(readBytes, expectWriteBytes);
}

TEST_F(TestFile, CreateFileReadWriteAppend) {
    string filename("/tmp/createFilerdwrappend.txt");
    remove(filename.c_str());    

    File file(filename, FILE_CREATE, FILE_READ_WRITE_APPEND);
 
    string filetext("123456789");
    char buffer[32];
    int readBytes;
    // int expectReadBytes = 0;
    EXPECT_EQ(file.read(buffer, 31, readBytes), FILE_SUCC);
    EXPECT_EQ(readBytes, 0);

    int writeBytes = 0;
    EXPECT_EQ(file.write(filetext.c_str(), filetext.length(), writeBytes), FILE_SUCC);
    int expectWriteBytes = filetext.length();
    EXPECT_EQ(writeBytes, expectWriteBytes);

    EXPECT_EQ(file.read(buffer, 31, readBytes), FILE_SUCC);
    EXPECT_EQ(readBytes, 0);
    file.seek(0);
    EXPECT_EQ(file.read(buffer, 31, readBytes), FILE_SUCC);
    EXPECT_EQ(readBytes, expectWriteBytes);

    file.close();
    EXPECT_EQ(file.write(filetext.c_str(), filetext.length(), writeBytes), FILE_ERR);
    EXPECT_EQ(file.read(buffer, 31, readBytes), FILE_ERR);

    // create an existing file, truncate it
    string filetext2("abcdefghijk");
    expectWriteBytes = filetext2.length();
    File file2(filename, FILE_CREATE, FILE_READ_WRITE_APPEND);
    EXPECT_EQ(file2.write(filetext2.c_str(), filetext2.length(), writeBytes), FILE_SUCC);
    EXPECT_EQ(writeBytes, expectWriteBytes);
    file2.close();
    
    File file3(filename, FILE_OPEN, FILE_READ_ONLY);
    EXPECT_EQ(file3.read(buffer, 31, readBytes), FILE_SUCC);
    EXPECT_EQ(readBytes, expectWriteBytes);
}

TEST_F(TestFile, OpenCreateFileReadOnly) {
    string filename("/tmp/opencreateFilerd.txt");
    remove(filename.c_str());    

    // file not exist, create it
    File file(filename, FILE_OPEN_CREATE, FILE_READ_ONLY);
    string filetext("123456789");   
    char buffer[32];
    int readBytes;
    EXPECT_EQ(file.read(buffer, 32, readBytes), FILE_SUCC);
    int expectReadBytes = 0;
    EXPECT_EQ(readBytes, expectReadBytes);  // empty file

    // can not write file as it is open read only
    int writeBytes = 0;
    EXPECT_EQ(file.write(filetext.c_str(), filetext.length(), writeBytes), FILE_ERR);
    file.close();
    EXPECT_EQ(file.read(buffer, 32, readBytes), FILE_ERR);

    // file exist, open it
    File file2(filename, FILE_CREATE, FILE_WRITE_ONLY);
    EXPECT_EQ(file2.write(filetext.c_str(), filetext.length(), writeBytes), FILE_SUCC);
    int expectWriteBytes = filetext.length();
    EXPECT_EQ(writeBytes, expectWriteBytes);
    File file3(filename, FILE_OPEN_CREATE, FILE_READ_ONLY);
    EXPECT_EQ(file3.read(buffer, 32, readBytes), FILE_SUCC);
    EXPECT_EQ(readBytes, expectWriteBytes);
    file2.close();
    file3.close();
}

TEST_F(TestFile, OpenCreateFileWriteOnly) {
    string filename("/tmp/opencreateFilewr.txt");
    remove(filename.c_str());    

    // file not exist, create it and write text
    File file(filename, FILE_OPEN_CREATE, FILE_WRITE_ONLY);
    string filetext("123456789");   
    char buffer[32];
    int readBytes;
    EXPECT_EQ(file.read(buffer, 32, readBytes), FILE_ERR);
    int writeBytes = 0;
    int expectWriteBytes = filetext.length();
    EXPECT_EQ(file.write(filetext.c_str(), filetext.length(), writeBytes), FILE_SUCC);
    EXPECT_EQ(writeBytes, expectWriteBytes);
    file.close();

    File file2(filename, FILE_OPEN_CREATE, FILE_READ_ONLY);
    EXPECT_EQ(file2.read(buffer, 32, readBytes), FILE_SUCC);
    EXPECT_EQ(readBytes, expectWriteBytes);
    file2.close();

    // file exist, open it and write new text, it will ovewrite from beginning
    string filetext2("abcd");
    expectWriteBytes = filetext2.length();
    File file3(filename, FILE_OPEN_CREATE, FILE_WRITE_ONLY);
    EXPECT_EQ(file3.write(filetext2.c_str(), filetext2.length(), writeBytes), FILE_SUCC);
    File file4(filename, FILE_OPEN_CREATE, FILE_READ_ONLY);
    EXPECT_EQ(file4.read(buffer, 32, readBytes), FILE_SUCC);
    if (filetext.length() > filetext2.length()) {
        EXPECT_GT(readBytes, expectWriteBytes);
        expectWriteBytes = filetext.length();
        EXPECT_EQ(readBytes, expectWriteBytes);
    } else {
        EXPECT_EQ(readBytes, expectWriteBytes);
    }
    file3.close();
    file4.close();
}

TEST_F(TestFile, OpenCreateFileReadWrite) {
    string filename("/tmp/opencreateFilerdwr.txt");
    remove(filename.c_str());    

    // file not exist, create it
    File file(filename, FILE_OPEN_CREATE, FILE_READ_WRITE);
    string filetext("123456789");   
    char buffer[32];
    int readBytes;
    EXPECT_EQ(file.read(buffer, 32, readBytes), FILE_SUCC);
    EXPECT_EQ(readBytes, 0);
    int writeBytes = 0;
    int expectWriteBytes = filetext.length();
    EXPECT_EQ(file.write(filetext.c_str(), filetext.length(), writeBytes), FILE_SUCC);
    EXPECT_EQ(writeBytes, expectWriteBytes);
    // read from the end, still no data
    EXPECT_EQ(file.read(buffer, 32, readBytes), FILE_SUCC);
    EXPECT_EQ(readBytes, 0);
    // read from beginning
    file.seek(0);
    int expectReadBytes = filetext.length();
    EXPECT_EQ(file.read(buffer, 32, readBytes), FILE_SUCC);
    EXPECT_EQ(readBytes, expectReadBytes);
    file.close();

    File file2(filename, FILE_OPEN_CREATE, FILE_READ_WRITE);
    EXPECT_EQ(file2.read(buffer, 32, readBytes), FILE_SUCC);
    EXPECT_EQ(readBytes, expectReadBytes);
    file2.close();

    // file exist, open it and write new text, it will ovewrite from beginning
    string filetext2("abcdefghijklmnopqrst");
    expectWriteBytes = filetext2.length();
    File file3(filename, FILE_OPEN_CREATE, FILE_READ_WRITE);
    EXPECT_EQ(file3.write(filetext2.c_str(), filetext2.length(), writeBytes), FILE_SUCC);
    file3.seek(0);
    EXPECT_EQ(file3.read(buffer, 32, readBytes), FILE_SUCC);
    if (filetext.length() > filetext2.length()) {
        EXPECT_GT(readBytes, expectWriteBytes);
        expectWriteBytes = filetext.length();
        EXPECT_EQ(readBytes, expectWriteBytes);
    } else {
        EXPECT_EQ(readBytes, expectWriteBytes);
    }
    file3.close();
}

TEST_F(TestFile, OpenCreateFileReadWriteAppend) {
    string filename("/tmp/opencreateFilerdwrappend.txt");
    remove(filename.c_str());    

    // file not exist, create it
    File file(filename, FILE_OPEN_CREATE, FILE_READ_WRITE_APPEND);
    string filetext("123456789");   
    char buffer[32];
    int readBytes;
    EXPECT_EQ(file.read(buffer, 32, readBytes), FILE_SUCC);
    EXPECT_EQ(readBytes, 0);
    int writeBytes = 0;
    int expectWriteBytes = filetext.length();
    EXPECT_EQ(file.write(filetext.c_str(), filetext.length(), writeBytes), FILE_SUCC);
    EXPECT_EQ(writeBytes, expectWriteBytes);
    // read from the end, still no data
    EXPECT_EQ(file.read(buffer, 32, readBytes), FILE_SUCC);
    EXPECT_EQ(readBytes, 0);
    // read from beginning
    file.seek(0);
    int expectReadBytes = filetext.length();
    EXPECT_EQ(file.read(buffer, 32, readBytes), FILE_SUCC);
    EXPECT_EQ(readBytes, expectReadBytes);
    file.close();

    // open exist file in read/write append mode, still read from the beginning
    File file2(filename, FILE_OPEN_CREATE, FILE_READ_WRITE_APPEND);
    EXPECT_EQ(file2.read(buffer, 32, readBytes), FILE_SUCC);
    EXPECT_EQ(readBytes, expectReadBytes);
    file2.close();

    // file exist, open it and write new text, it will write from the end
    string filetext2("abcdefghijk");
    expectWriteBytes = filetext2.length();
    File file3(filename, FILE_OPEN_CREATE, FILE_READ_WRITE_APPEND);
    EXPECT_EQ(file3.write(filetext2.c_str(), filetext2.length(), writeBytes), FILE_SUCC);
    file3.seek(0);
    EXPECT_EQ(file3.read(buffer, 32, readBytes), FILE_SUCC);    
    expectReadBytes = filetext.length() + filetext2.length();
    EXPECT_EQ(readBytes, expectReadBytes);
    file3.close();
}

