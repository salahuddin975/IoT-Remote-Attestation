#include <sstream>
#include <algorithm>
#include <iterator>
#include <time.h>
#include <stdlib.h>
#include <wiringPi.h>

#include "zone_controller.hpp"
#include "server.hpp"


ZoneController::ZoneController(Server& comms)
: comms_(comms)
 {
    wiringPiSetup();
    commands_["on"] = &ZoneController::On;
    commands_["off"] = &ZoneController::Off;
    commands_["status"] = &ZoneController::Status;

    // zones_ should be read from a configuration file here
    zones_.push_back({"citrus", 2, 0, false});
    zones_.push_back({"grass", 3, 0, false});
    zones_.push_back({"vedgie", 4, 0, false});
    zones_.push_back({"side", 5, 0, false});

    master_.name = "MASTER";
    master_.pin = 0;
    master_.currentlyOn = false;

    pinMode(master_.pin, OUTPUT);
    digitalWrite(master_.pin, HIGH);

    for (int j = 0; j < zones_.size(); j++)
    {
        Zone& z(zones_[j]);
        pinMode(z.pin, OUTPUT);
        digitalWrite(z.pin, HIGH);
    }
}

int ZoneController::ParseLine(int filedes, std::string& sentence)
{
    std::istringstream iss(sentence);
    std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                      std::istream_iterator<std::string>{}};
    if (tokens.size() > 0)
    {
        if (commands_.find(tokens.front()) != commands_.end())
        {
            (this->*(commands_[tokens.front()]))(filedes, tokens);
        }
	else
	{
	    comms_.PrintfSock(filedes, "No command %s\r\n", tokens.front().c_str());
	}
    }
}

void ZoneController::On(int filedes, std::vector<std::string>& line)
{
  if (line.size() == 3)
  {
    bool found = false;
    int i = 0;
    for (; i < zones_.size() && found == false; i++)
    {
      found = line[1] == zones_[i].name;
    }
    long seconds = strtol(line[2].c_str(), NULL, 10);

    if (found == false)
    {
      comms_.PrintfSock(filedes, "Unable to find zone %s.\r\n", line[1].c_str());
    }
    else
    {
      i--;
      for (int j = 0; j < zones_.size(); j++)
      {
        if ((i != j) && zones_[j].secondsRemaining > 0)
        {
          zones_[j].secondsRemaining = 0;
          comms_.PrintfSock(filedes, "Forced zone %s off, ", zones_[j].name.c_str());
        }
      }

      zones_[i].secondsRemaining = seconds;
      comms_.PrintfSock(filedes, "Zone %s on for %i.\r\n", zones_[i].name.c_str(), zones_[i].secondsRemaining);
    }
  }
  else
  {
    comms_.PrintfSock(filedes, "Bad arguements to On.\r\n");
  }
}

void ZoneController::Off(int filedes, std::vector<std::string>& line)
{
  if (line.size() == 2)
  {
    bool found = false;
    int i = 0;
    for (; i < zones_.size() && found == false; i++)
    {
      found = line[1] == zones_[i].name;
    }

    if (found == false)
    {
      comms_.PrintfSock(filedes, "Unable to find zone %s.\r\n", line[1].c_str());
    }
    else
    {
      i--;
      zones_[i].secondsRemaining = 0;
      comms_.PrintfSock(filedes, "Zone %s off.\r\n", zones_[i].name.c_str());
    }
  }
  else
  {
    comms_.PrintfSock(filedes, "Bad arguements to On.\r\n");
  }
}

void ZoneController::Status(int filedes, std::vector<std::string>& line)
{
    for (int j = 0; j < zones_.size(); j++)
    {
      comms_.PrintfSock(filedes, "(%s, %i)", zones_[j].name.c_str(), zones_[j].secondsRemaining);
    }
    comms_.PrintfSock(filedes, "\n\r");
}

void ZoneController::Periodic(void)
{
  static time_t lastSeconds = time(NULL);

  time_t thisTime = time(NULL);
  time_t delta = thisTime - lastSeconds;
  lastSeconds = thisTime;
  bool oneOn = false;

 // after the system is disconnected from the mains, let one of the zones
 // release the preassure on the manafold by opening it for some amount of
 // time.
  pressureRelease -= delta;

  // first shut them down
  for (int i = 0; i < zones_.size(); i++)
  {
    Zone& z(zones_[i]);
    z.secondsRemaining -= delta;
    if (z.secondsRemaining <= 0)
    {
      z.secondsRemaining = 0;
      // don't shut a zone if it is releaving preassure in the manafold
      if (z.currentlyOn && (pressureRelease < 0 || pressureZone != i))
      {
        Close(z);
        z.currentlyOn = false;
      }
    }
  }

  for (int i = 0; i < zones_.size(); i++)
  {
    Zone& z(zones_[i]);
    if (z.secondsRemaining > 0)
    {
      oneOn = true;
      pressureRelease = 0;
      if (z.currentlyOn == false)
      {
        Open(z);
      }
    }
  }

  if (oneOn)
  {
    if (master_.currentlyOn == false)
    {
      Open(master_);
    }
  }
  else
  {
    if (master_.currentlyOn == true)
    {
      Close(master_);
      pressureRelease = 20;
      Open(zones_[pressureZone]);
    }
  }


}

void ZoneController::ShutAllValves(void)
{
    comms_.PrintfAllSockets("Shutting all values\r\n");
    Close(master_);
    for (int i = 0; i < zones_.size(); i++)
    {
        Close(zones_[i]);
    }
}

void ZoneController::Open(Zone & z)
{
    comms_.PrintfAllSockets("Opening %s, pin %i\r\n", z.name.c_str(), z.pin);
    digitalWrite(z.pin, LOW);
    z.currentlyOn = true;
}

void ZoneController::Close(Zone & z)
{
    comms_.PrintfAllSockets("Shutting %s, pin %i\r\n", z.name.c_str(), z.pin);
    digitalWrite(z.pin, HIGH);
    z.currentlyOn = false;
}
