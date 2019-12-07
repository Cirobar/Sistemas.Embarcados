#include <stdio.h>
#include <tgbot/tgbot.h>
#include<fstream>
#include <iostream>
#include <typeinfo>
#include<vector>
using namespace std;


int exec=0;
int filerd()
{

    system("ls /dev | grep video0 >> readcam.txt");
	ifstream file;
    string inputString;
	vector<string> data;

	file.open("readcam.txt");
	while(file>>inputString) //reads one string at a time
		  data.push_back(inputString); //add it to data vector

  if((inputString.compare("video0")) == 0){
      exec=1;
  }
  else {
      exec=0;
  }
  file.close();
  system("rm readcam.txt");
  return 0;
}

int filewr() {
  ofstream myfile;
  myfile.open ("stream.txt",ios::trunc);
  myfile << "1";
  myfile.close();
  return 0;
}

int filewrc() {
  ofstream myfile;
  myfile.open ("stream.txt",ios::trunc);
  myfile << "0";
  myfile.close();
  return 0;
}

int main() {

    ifstream file;
    filewrc();
    TgBot::Bot bot("106");
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hello there!");
    });
    bot.getEvents().onCommand("open",[&bot](TgBot::Message::Ptr message) {

        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }
        filerd();
        if(exec == 1){
              bot.getApi().sendMessage(message->chat->id, "A stream já está aberta, para fechar use o comando /close");
        }
        else{
              system("uv4l --driver raspicam --auto-video_nr --width 640 --height 480 --encoding jpeg");
              usleep(100);
              system("dd if=/dev/video0 of=uv4image.jpeg bs=11M count=1");
              usleep(100);
              bot.getApi().sendMessage(message->chat->id, "A stream foi iniciada!");
              filewr();
        }
    });
    bot.getEvents().onCommand("close",[&bot](TgBot::Message::Ptr message) {
        //printf("User wrote %s\n", message->text.c_str());
        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }
        system("sudo pkill uv4l");
        bot.getApi().sendMessage(message->chat->id, "A stream foi encerrada!");
        filewrc();
    });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");  //espera comando por telegram
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
    return 0;
}
