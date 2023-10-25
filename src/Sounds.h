
#ifndef PLATFORMERSFML_SOUNDS_H
#define PLATFORMERSFML_SOUNDS_H

#include <SFML/Audio.hpp>

class Sound
{
 public:
  sf::Sound coin_sound;
  sf::SoundBuffer coin_sound_buffer;
  sf::Sound get_sword_sound;
  sf::SoundBuffer get_sword_sound_buffer;
  sf::Sound attack_sound;
  sf::SoundBuffer attack_sound_buffer;
  sf::Sound death_sound;
  sf::SoundBuffer death_sound_buffer;
  sf::Sound jump_sound;
  sf::SoundBuffer jump_sound_buffer;
  sf::Sound menu_sound;
  sf::SoundBuffer menu_sound_buffer;

  sf::Sound title_music;
  sf::SoundBuffer title_music_buffer;
  sf::Sound game_music;
  sf::SoundBuffer game_music_buffer;
};

#endif // PLATFORMERSFML_SOUNDS_H
