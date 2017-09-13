#include <memory>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
//#include <SFML/Network.hpp>
#include <android/log.h>


#include "Test.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "native-activity", __VA_ARGS__))


int main(int argc, char *argv[]) {

  std::vector<int> vi;
  std::unique_ptr<sf::RenderWindow> upW(new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "OMG!!!"));
  //sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "OMG!!!");
  sf::RenderWindow& window = *upW;

  sf::Texture texture;
  if (!texture.loadFromFile("image.png")) {
    LOGE("TEX not found");
    return EXIT_FAILURE;
  }

  sf::Sprite spr(texture);

  spr.setPosition(sf::Vector2f(50.f, 50.f));

  /*sf::Music music;
  if(!music.openFromFile("msx/relax.ogg")) {
      LOGE("[NO MUSIC]");
      return EXIT_FAILURE;
  }
  music.play();
  */

  sf::SoundBuffer buffer;
  if (!buffer.loadFromFile("canary.wav")) {
    LOGE("SND not found");
    return EXIT_FAILURE;
  }

  sf::Sound sound;
  sound.setBuffer(buffer);

  sf::View view = window.getDefaultView();

  while (window.isOpen())
  {
    sf::Event event;

    while (window.pollEvent(event))
    {
      switch (event.type)
      {
        case sf::Event::Closed:
          window.close();
          break;
        case sf::Event::Resized: {
          LOGI("[EVT] - resized!!!");
            view.setSize(event.size.width, event.size.height);
            view.setCenter(event.size.width / 2, event.size.height / 2);
            window.setView(view);
        } break;
        case sf::Event::TouchBegan: {
            LOGI("[EVT] - touch!!!");
          if (event.touch.finger == 0) {
            spr.setPosition(event.touch.x, event.touch.y);
            sound.play();
          }
        } break;

        default: break;
      }
    }
    window.clear(sf::Color::Black);
    window.draw(spr);
    window.display();
  }
}