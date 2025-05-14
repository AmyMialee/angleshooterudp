#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "Windows.h"
#include "WinSock2.h"
#include "Ws2tcpip.h"
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#include <chrono>
#include <corecrt_math_defines.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <thread>
#include <algorithm>
#include <numbers>
#include <mutex>
#include <queue>
#include <utility>
#include <regex>
#include <bitset>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "../nlohmann/json.hpp"

#include "util/Consumer.h"
#include "util/Identifier.h"
#include "util/Logger.h"
#include "util/Util.h"
#include "util/StackMove.h"
#include "util/PlayerCosmetics.h"
#include "network/BitStream.h"
#include "network/PacketIdentifier.h"
#include "network/PortedIP.h"
#include "network/SocketHolder.h"
#include "network/NetworkPair.h"
#include "AngleShooterCommon.h"
#include "NetworkProtocol.hpp"

#include "resources/MapLoader.h"
#include "game/entities/Entity.h"
#include "game/World.h"
#include "game/entities/PlayerEntity.h"
#include "game/entities/BulletEntity.h"