#pragma once

class NetworkProtocol {
public:
	inline static PacketIdentifier* PING;
	inline static PacketIdentifier* PONG;
	inline static PacketIdentifier* ACK;
	inline static PacketIdentifier* CHAT_MESSAGE;
	inline static PacketIdentifier* SERVER_SCAN;

	inline static PacketIdentifier* S2C_INITIAL_SETUP;
	inline static PacketIdentifier* S2C_BROADCAST_MESSAGE;
	inline static PacketIdentifier* S2C_PLAY_MUSIC;
	inline static PacketIdentifier* S2C_PLAY_SOUND;
	inline static PacketIdentifier* S2C_PLAY_SOUND_3D;
	inline static PacketIdentifier* S2C_LOAD_MAP;
	inline static PacketIdentifier* S2C_SPAWN_PLAYER;
	inline static PacketIdentifier* S2C_SPAWN_BULLET;
	inline static PacketIdentifier* S2C_PLAYER_INPUT;
	inline static PacketIdentifier* S2C_PLAYER_POSITION_SYNC;
	inline static PacketIdentifier* S2C_BULLET_CHARGE;
	inline static PacketIdentifier* S2C_HEALTH;
	inline static PacketIdentifier* S2C_DEATH;
	inline static PacketIdentifier* S2C_TELEPORT;
	inline static PacketIdentifier* S2C_REMOVE_OBJECT;
	inline static PacketIdentifier* S2C_UPDATE_SCORE;
	inline static PacketIdentifier* S2C_UPDATE_NAME;
	inline static PacketIdentifier* S2C_UPDATE_COLOUR;

	inline static PacketIdentifier* C2S_JOIN;
	inline static PacketIdentifier* C2S_SEND_MESSAGE;
	inline static PacketIdentifier* C2S_QUIT;
	inline static PacketIdentifier* C2S_PLAYER_INPUT;
	inline static PacketIdentifier* C2S_PLAYER_POSITION_SYNC;
	inline static PacketIdentifier* C2S_UPDATE_NAME;
	inline static PacketIdentifier* C2S_UPDATE_COLOUR;
	
	static void initialize() {
		PING = PacketIdentifier::of(true, "PING");
		PONG = PacketIdentifier::of(true, "PONG");
		ACK = PacketIdentifier::of(false, "ACK");
		CHAT_MESSAGE = PacketIdentifier::of(true, "CHAT_MESSAGE");
		SERVER_SCAN = PacketIdentifier::of(false, "SERVER_SCAN");
		
		S2C_INITIAL_SETUP = PacketIdentifier::of(true, "S2C_INITIAL_SETUP");
		S2C_BROADCAST_MESSAGE = PacketIdentifier::of(true, "S2C_BROADCAST_MESSAGE");
		S2C_PLAY_MUSIC = PacketIdentifier::of(true, "S2C_PLAY_MUSIC");
		S2C_PLAY_SOUND = PacketIdentifier::of(true, "S2C_PLAY_SOUND");
		S2C_PLAY_SOUND_3D = PacketIdentifier::of(true, "S2C_PLAY_SOUND_3D");
		S2C_LOAD_MAP = PacketIdentifier::of(true, "S2C_LOAD_MAP");
		S2C_SPAWN_PLAYER = PacketIdentifier::of(true, "S2C_SPAWN_PLAYER");
		S2C_SPAWN_BULLET = PacketIdentifier::of(true, "S2C_SPAWN_BULLET");
		S2C_PLAYER_INPUT = PacketIdentifier::of(false, "S2C_PLAYER_INPUT");
		S2C_PLAYER_POSITION_SYNC = PacketIdentifier::of(false, "S2C_PLAYER_POSITION_SYNC");
		S2C_BULLET_CHARGE = PacketIdentifier::of(false, "S2C_BULLET_CHARGE");
		S2C_HEALTH = PacketIdentifier::of(false, "S2C_HEALTH");
		S2C_DEATH = PacketIdentifier::of(true, "S2C_DEATH");
		S2C_TELEPORT = PacketIdentifier::of(true, "S2C_TELEPORT");
		S2C_REMOVE_OBJECT = PacketIdentifier::of(true, "S2C_REMOVE_OBJECT");
		S2C_UPDATE_SCORE = PacketIdentifier::of(true, "S2C_UPDATE_SCORE");
		S2C_UPDATE_NAME = PacketIdentifier::of(true, "S2C_UPDATE_NAME");
		S2C_UPDATE_COLOUR = PacketIdentifier::of(true, "S2C_UPDATE_COLOUR");

		C2S_JOIN = PacketIdentifier::of(true, "C2S_JOIN");
		C2S_SEND_MESSAGE = PacketIdentifier::of(true, "C2S_SEND_MESSAGE");
		C2S_QUIT = PacketIdentifier::of(true, "C2S_QUIT");
		C2S_PLAYER_INPUT = PacketIdentifier::of(true, "C2S_PLAYER_INPUT");
		C2S_PLAYER_POSITION_SYNC = PacketIdentifier::of(false, "C2S_PLAYER_POSITION_SYNC");
		C2S_UPDATE_NAME = PacketIdentifier::of(true, "C2S_UPDATE_NAME");
		C2S_UPDATE_COLOUR = PacketIdentifier::of(true, "C2S_UPDATE_COLOUR");
	}
};