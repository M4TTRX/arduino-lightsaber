
#include "SoftwareSerial.h"

// USE SERIAL COM 9600

#define AUDIO_TX_PIN 2
#define AUDIO_RX_PIN 4

// start_byte is the byte that must at the start
// all command.
#define start_byte 0x7E

// version_byte describes the version of the system
// we are using.
#define version_byte 0xFF

// end_byte is the byte that must be added at the
// end of every comand.
#define end_byte 0xEF

// message_length describes the number of bytes
// in the body of the command.It can be up to 6
// bytes long so we will keep all commands at 6
// bytes long for simplicity's sake.
#define message_length 0x06

#define ack 0x00
// ---------------------------------------------

// COMANDS

#define PLAY_CMD 0x0D
#define PAUSE_CMD 0x0E
#define SET_VOLUME_CMD 0x06
#define REPEAT_PLAY_CMD 0x11 // 1: start repeat play, 0: stop play
#define SEND_INIT_CMD 0x3F   // 0 -0x0F(each bit represent one device of the low-four bits)

// Set up the audio serial
SoftwareSerial mySerial(AUDIO_TX_PIN, AUDIO_RX_PIN);

void execute_cmd(byte cmd, byte Par1, byte Par2)
// Excecute the command and parameters
{
    // Calculate the checksum (2 bytes)
    word checksum = -(version_byte + message_length + cmd + ack + Par1 + Par2);

    // Build the command line
    byte Command_line[10] = {start_byte, version_byte, message_length, cmd, ack,
                             Par1, Par2, highByte(checksum), lowByte(checksum), end_byte};
    //Send the command line to the module
    for (byte k = 0; k < 10; k++)
    {
        mySerial.write(Command_line[k]);
    }
}

void set_volume(int volume)
{
    // Set the volume (0x00~0x30)
    execute_cmd(SET_VOLUME_CMD, 0, volume);
    delay(2000);
}

void play_first()
{
    execute_cmd(SEND_INIT_CMD, 0, 0);
    delay(500);
    set_volume(0x30);
    delay(500);
    execute_cmd(REPEAT_PLAY_CMD, 0, 1);
    delay(500);
}

void pause()
{
    execute_cmd(PAUSE_CMD, 0, 0);
    delay(500);
}

void play()
{
    execute_cmd(0x0D, 0, 1);
    delay(500);
}
