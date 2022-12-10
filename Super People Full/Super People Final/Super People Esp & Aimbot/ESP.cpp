// Extra Sensory Perception (ESP)
#include "pch.h"

ESP* g_pESP = new ESP();
ESP::ESP() {}
ESP::~ESP() {}

sf::Vertex line[2];
sf::VertexArray armsLegs(sf::LinesStrip, 7);
sf::VertexArray body(sf::LinesStrip, 5);
sf::CircleShape playerHead(30.f);
sf::Font font;
sf::Text playerName;
sf::RectangleShape healthBar;
sf::RectangleShape healthBorder;
sf::RectangleShape infoBg;
sf::Text playerInfo;
sf::Text itemName;
sf::RectangleShape itemBg;
sf::Music alert;

bool ESP::loadFont()
{
	if (!font.loadFromFile("./PUBGCorona.ttf"))
	{
		Sleep(1000);
		if (font.loadFromFile("PUBGCorona.ttf"))
		{
			return true;
		}
		return false;
	}
	return true;
}

void ESP::setDrawings()
{
	line[0].color = sf::Color::Red;
	line[0].position = sf::Vector2f(g_pOverlay->width / 2, 0.f);
	line[1].color = sf::Color::Red;
	line[1].position = sf::Vector2f(0.f, 0.f);
	//g_pOverlay->window.draw(line, 2, sf::Lines);

	//line[1].position = sf::Vector2f(200.f, 350.f);
	//g_pOverlay->window.draw(line, 2, sf::Lines);

	armsLegs[0].color = sf::Color::Red;
	armsLegs[1].color = sf::Color::Red;
	armsLegs[2].color = sf::Color::Red;
	armsLegs[3].color = sf::Color::Red;
	armsLegs[4].color = sf::Color::Red;
	armsLegs[5].color = sf::Color::Red;
	armsLegs[6].color = sf::Color::Red;
	armsLegs[0].position = sf::Vector2f(160.f, 370.f);
	armsLegs[1].position = sf::Vector2f(135.f, 305.f);
	armsLegs[2].position = sf::Vector2f(170.f, 240.f);
	armsLegs[3].position = sf::Vector2f(205.f, 220.f);
	armsLegs[4].position = sf::Vector2f(240.f, 240.f);
	armsLegs[5].position = sf::Vector2f(275.f, 305.f);
	armsLegs[6].position = sf::Vector2f(250.f, 370.f);
	//g_pOverlay->window.draw(armsLegs);

	body[0].color = sf::Color::Red;
	body[1].color = sf::Color::Red;
	body[2].color = sf::Color::Red;
	body[3].color = sf::Color::Red;
	body[4].color = sf::Color::Red;
	body[0].position = sf::Vector2f(205.f, 220.f);
	body[1].position = sf::Vector2f(215.f, 250.f);
	body[2].position = sf::Vector2f(200.f, 305.f);
	body[3].position = sf::Vector2f(215.f, 355.f);
	body[4].position = sf::Vector2f(205.f, 375.f);
	//g_pOverlay->window.draw(body);

	//armsLegs[0].position = sf::Vector2f(185.f, 565.f);
	//armsLegs[1].position = sf::Vector2f(165.f, 475.f);
	//armsLegs[2].position = sf::Vector2f(180.f, 405.f);
	//armsLegs[3].position = sf::Vector2f(205.f, 375.f);
	//armsLegs[4].position = sf::Vector2f(235.f, 405.f);
	//armsLegs[5].position = sf::Vector2f(250.f, 475.f);
	//armsLegs[6].position = sf::Vector2f(230.f, 565.f);
	//g_pOverlay->window.draw(armsLegs);

	playerHead.setPosition(205.f, 220.f);
	playerHead.setOutlineColor(sf::Color::Red);
	playerHead.setOutlineThickness(1.f);
	playerHead.setFillColor(sf::Color::Transparent);
	playerHead.setOrigin(30.f, (30.f * 2) + 1.f);
	//g_pOverlay->window.draw(playerHead);

	infoBg.setPosition(sf::Vector2f(200.f, 350.f));
	infoBg.setFillColor(sf::Color(0, 0, 0, 100));
	infoBg.setSize(sf::Vector2f(202.f, 36.f));
	infoBg.setOrigin(sf::Vector2f(101.f, 250.f));
	//g_pOverlay->window.draw(infoBg);

	playerName.setFont(font);
	playerName.setString("Player Name\nStatus");
	playerName.setFillColor(sf::Color::Magenta);
	playerName.setCharacterSize(12);
	playerName.setLineSpacing(0.5);
	playerName.setPosition(sf::Vector2f(200.f, 350.f));
	playerName.setOrigin(sf::Vector2f(-50.f + 101.f, -1.f + 250.f));
	//g_pOverlay->window.draw(playerName);

	healthBar.setPosition(sf::Vector2f(200.f, 350.f));
	healthBar.setFillColor(sf::Color::Green);
	healthBar.setSize(sf::Vector2f(50.f * 2.f, 4.f));
	healthBar.setOrigin(sf::Vector2f(-1.f + 101.f, -31.f + 250.f));
	//g_pOverlay->window.draw(healthBar);

	healthBorder.setPosition(sf::Vector2f(200.f, 350.f));
	healthBorder.setOutlineColor(sf::Color::White);
	healthBorder.setSize(sf::Vector2f(200.f, 4.f));
	healthBorder.setOutlineThickness(1.f);
	healthBorder.setFillColor(sf::Color::Transparent);
	healthBorder.setOrigin(sf::Vector2f(-1.f + 101.f, -31.f + 250.f));
	//g_pOverlay->window.draw(healthBorder);

	playerInfo.setFont(font);
	playerInfo.setFillColor(sf::Color::White);
	playerInfo.setString("500\nm");
	playerInfo.setLineSpacing(0.5);
	playerInfo.setCharacterSize(12);
	playerInfo.setPosition(sf::Vector2f(200.f, 350.f));
	playerInfo.setOrigin(sf::Vector2f(-1.f + 101.f, -1.f + 250.f));
	//g_pOverlay->window.draw(playerInfo);

	//playerInfo.setString("50\nhp");
	//playerInfo.setOrigin(sf::Vector2f(-180.f + 101.f, -1.f + 250.f));
	//g_pOverlay->window.draw(playerInfo);


	//line[1].position = sf::Vector2f(600.f, 300.f);
	//g_pOverlay->window.draw(line, 2, sf::Lines);

	itemName.setFont(font);
	itemName.setString("Item (250 m)");
	itemName.setFillColor(sf::Color::Red);
	itemName.setCharacterSize(12);
	itemName.setPosition(sf::Vector2f(600.f, 300.f));
	itemName.setOutlineThickness(2);
	itemName.setOutlineColor(sf::Color::Black);

	sf::FloatRect itemBounds;
	itemBounds = itemName.getGlobalBounds();
	itemName.setOrigin(sf::Vector2f(itemBounds.width / 2, 10.f));
	itemBounds = itemName.getGlobalBounds();

	itemBg.setPosition(itemBounds.left, itemBounds.top);
	itemBg.setFillColor(sf::Color(0, 0, 0, 100));
	itemBg.setSize(sf::Vector2f(itemBounds.width + 10.f, itemBounds.height + 10.f));
	itemBg.setOrigin(sf::Vector2f(5.f, 5.f));
	//g_pOverlay->window.draw(itemBg);
	//g_pOverlay->window.draw(itemName);

	/*
	if (playAlert)
	{
		alert.openFromFile("./PUBGAlert.wav");
		alert.setVolume(80);
		alert.play();
		playAlert = false;
	}
	*/
}

void ESP::init()
{
	cout << "[+] Getting view world ...\n";
	viewWorldBase = getViewWorld();
	if (viewWorldBase == 0)
	{
		cout << "FAILED ...\n";
		system("pause");
		return;
	}
	cout << "SUCCESS ...\n";
	cout << "View world base is : " << viewWorldBase << "\n";
	uWorld = viewWorldBase - ofUWorldMinus;
	cout << "uWorld is : " << uWorld << "\n";
	gNames = viewWorldBase - ofGNamesMinus;
	cout << "gNames is : " << gNames << "\n";
	tmpGName = g_pMemoryManager->dRead(gNames);
	cout << "tmpGName is : " << tmpGName << "\n";
}

DWORD ESP::getViewWorld()
{
	BYTE pattern[] = { 0x02, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x01 };
	vector<DWORD_PTR> foundedBases;
	g_pMemoryManager->search(pattern, sizeof(pattern), ofSearchFrom, ofSearchTo, foundedBases);
	for (int i = 0; i < foundedBases.size(); i++) {
		DWORD cand = foundedBases[i] - 0x20;
		DWORD eng = g_pMemoryManager->dRead(cand) + 0x20;
		DWORD tmp = g_pMemoryManager->dRead(eng) + 0x200;
		float v1, v2, v3, v4;
		v1 = g_pMemoryManager->fRead(tmp + 0x8);
		v2 = g_pMemoryManager->fRead(tmp + 0x18);
		v3 = g_pMemoryManager->fRead(tmp + 0x28);
		v4 = g_pMemoryManager->fRead(tmp + 0x38);
		if ((v1 == 0) && (v2 == 0) && (v3 == 0) && (v4 >= 3)) {
			foundedBases.clear();
			return cand;
		}
	}
	foundedBases.clear();
	return 0;
}

void ESP::getViewMatrix()
{
	DWORD vwb20 = g_pMemoryManager->dRead(viewWorldBase) + 0x20;
	DWORD vwb200 = g_pMemoryManager->dRead(vwb20) + 0x200;
	g_pMemoryManager->readMemory((PVOID)vwb200, (PVOID)&g_pViewMatrixManager->viewmatrix, 64);
}


void ESP::scanEntityList()
{
	uWorlds = g_pMemoryManager->dRead(uWorld);
	uLevel = g_pMemoryManager->dRead(uWorlds + ofULevel);
	gameInstance = g_pMemoryManager->dRead(uWorlds + ofGameInstance);

	if (gameInstance == 0)
	{
		cout << "Error : Game instance error\n";
		Sleep(500);
		return;
	}

	playerController = g_pMemoryManager->dRead(gameInstance + ofPlayerController);
	playerCarry = g_pMemoryManager->dRead(playerController + ofPlayerCarry);
	uMyObject = g_pMemoryManager->dRead(playerCarry + ofUMyObject);
	entityEntry = g_pMemoryManager->dRead(uLevel + ofEntityEntry);
	entityCount = g_pMemoryManager->iRead(uLevel + ofEntityCount);

	enemies = 0;

	for (int ent = 0; ent < entityCount; ent++)
	{
		entityAddv = g_pMemoryManager->dRead(entityEntry + (ent * ofEntityAddv));

		if (entityAddv == 0) { continue; }
		if (entityAddv == -1) { continue; }
		if (!entityAddv) { continue; }
		if (entityAddv == NULL) { continue; }
		if (entityAddv == (DWORD) nullptr) { continue; }

		tmpEntityAddvId = g_pMemoryManager->uiRead(entityAddv + ofTmpEntityAddvId);

		entityName = "";

		if ((tmpEntityAddvId > 0) && (tmpEntityAddvId < 50000000))
		{
			page = tmpEntityAddvId / 16384; // 0x4000
			index = tmpEntityAddvId % 16384;
			secPartAddv = g_pMemoryManager->dRead(tmpGName + page * 4);
			if (secPartAddv > 0)
			{
				nameAddv = g_pMemoryManager->dRead(secPartAddv + index * 4);
				if (nameAddv > 0)
				{
					entityName = g_pMemoryManager->charRead(nameAddv + 8);
				}
			}
		}

		if (entityName == "")
		{
			continue;
		}

		// ITEMS
		if (
			strstr(entityName.c_str(), "BP_Grenade") ||
			strstr(entityName.c_str(), "_Wrapper") || strstr(entityName.c_str(), "BP_WEP_Mk14_Pickup_") ||
			strstr(entityName.c_str(), "BP_Ammo") ||
			strstr(entityName.c_str(), "BP_MZJ") ||
			strstr(entityName.c_str(), "PickUp_BP") ||
			strstr(entityName.c_str(), "_Pickup") ||
			strstr(entityName.c_str(), "PlayerDeadInventoryBox_") ||
			strstr(entityName.c_str(), "PickUpListWrapperActor") ||
			strstr(entityName.c_str(), "BP_AirDropBox_") ||
			strstr(entityName.c_str(), "AirDropListWrapperActor") ||
			strstr(entityName.c_str(), "VH_") ||
			strstr(entityName.c_str(), "BP_AirDropPlane_") ||
			strstr(entityName.c_str(), "LadaNiva") ||
			strstr(entityName.c_str(), "AquaRail") ||
			strstr(entityName.c_str(), "Rony") ||
			strstr(entityName.c_str(), "Mirado") ||
			strstr(entityName.c_str(), "PickUp_")
			)
		{
			entityWorld = g_pMemoryManager->dRead(entityAddv + ofEntityWorld);
			position = g_pMemoryManager->v3Read(entityWorld + ofPosition);
			g_pViewMatrixManager->worldToScreen(position, positionOnScreen, &distanceFl, &distanceValue);

			if (positionOnScreen.z < 0.001f)
			{
				continue;
			}

			// THROWABLES (BP_Grenade_Frag_Weapon_Wrapper_C)
			if (strstr(entityName.c_str(), "BP_Grenade"))
			{
				entityName = replace(entityName, "BP_Grenade_");
				entityName = replace(entityName, "_Weapon_Wrapper_C");
				itemName.setFillColor(sf::Color::Red);
			}
			// WEAPONS (BP_Sniper_M24_Wrapper_C, BP_WEP_Mk14_Pickup_C)
			else if (strstr(entityName.c_str(), "_Wrapper") || strstr(entityName.c_str(), "BP_WEP_Mk14_Pickup_"))
			{
				entityName = replace(entityName, "BP_");
				entityName = replace(entityName, "_Wrapper_C");
				entityName = replace(entityName, "_Pickup_C");
				itemName.setFillColor(sf::Color::Yellow);
			}
			// AMMO (BP_Ammo_556mm_Pickup_)
			else if (strstr(entityName.c_str(), "BP_Ammo"))
			{
				entityName = replace(entityName, "BP_Ammo_");
				entityName = replace(entityName, "_Pickup_C");
				itemName.setFillColor(sf::Color::Green);
			}
			// SCOPES (BP_MZJ_4X_Pickup_)
			else if (strstr(entityName.c_str(), "BP_MZJ"))
			{
				entityName = replace(entityName, "BP_MZJ_");
				entityName = replace(entityName, "_Pickup_C");
				itemName.setFillColor(sf::Color::White);
			}
			// WEARINGS (PickUp_BP_Bag_Lv2_)
			else if (strstr(entityName.c_str(), "PickUp_BP"))
			{
				entityName = replace(entityName, "PickUp_BP_");
				entityName = replace(entityName, "_C");
				itemName.setFillColor(sf::Color::Blue);
			}
			// HEALINGS (Bandage_Pickup_)
			// ATTACHMENTS (BP_QK_Mid_FlashHider_Pickup_)
			// METAL WEAPONS (BP_WEP_Pan_Pickup_)
			// GAS CAN (GasCan_Destructible_Pickup_)
			else if (strstr(entityName.c_str(), "_Pickup"))
			{
				entityName = replace(entityName, "_Pickup_C");
				entityName = replace(entityName, "BP_WEP_");
				entityName = replace(entityName, "BP_");
				itemName.setFillColor(sf::Color::Cyan);
			}
			// BOXES
			else if (
				strstr(entityName.c_str(), "PlayerDeadInventoryBox_") ||
				strstr(entityName.c_str(), "PickUpListWrapperActor") ||
				strstr(entityName.c_str(), "BP_AirDropBox_") ||
				strstr(entityName.c_str(), "AirDropListWrapperActor")
				)
			{
				itemName.setFillColor(sf::Color(255, 165, 0, 255));
			}
			// VEHICLES (BP_VH_Buggy_, BP_AirDropPlane_, LadaNiva_01_, AquaRail_1_, Rony_01_, Mirado_open_4_, PickUp_02_)
			else if (
				strstr(entityName.c_str(), "VH_") ||
				strstr(entityName.c_str(), "BP_AirDropPlane_") ||
				strstr(entityName.c_str(), "LadaNiva") ||
				strstr(entityName.c_str(), "AquaRail") ||
				strstr(entityName.c_str(), "Rony") ||
				strstr(entityName.c_str(), "Mirado") ||
				strstr(entityName.c_str(), "PickUp_")
				)
			{
				entityName = replace(entityName, "BP_");
				entityName = replace(entityName, "VH_");
				entityName = replace(entityName, "_C");
				itemName.setFillColor(sf::Color::Magenta);
			}

			itemName.setString(entityName + " (" + to_string(distanceValue) + " m)");
			itemName.setPosition(sf::Vector2f(positionOnScreen.x, positionOnScreen.y));
			sf::FloatRect itemBounds;
			itemBounds = itemName.getGlobalBounds();
			itemName.setOrigin(sf::Vector2f(itemBounds.width / 2, 10.f));
			itemBounds = itemName.getGlobalBounds();
			itemBg.setPosition(itemBounds.left, itemBounds.top);
			itemBg.setSize(sf::Vector2f(itemBounds.width + 10.f, itemBounds.height + 10.f));
			g_pOverlay->window.draw(itemBg);
			g_pOverlay->window.draw(itemName);
		}
		// PLAYERS
		else if (strstr(entityName.c_str(), "BP_PlayerPawn") || strstr(entityName.c_str(), "BP_PlayerCharacter") || strstr(entityName.c_str(), "PlanET_FakePlayer") || strstr(entityName.c_str(), "PlayerPawn_Infec"))
		{
			entityWorld = g_pMemoryManager->dRead(entityAddv + ofEntityWorld);

			status = g_pMemoryManager->iRead(entityWorld + ofStatus);
			if (status == 6)
			{
				continue;
			}

			teamId = g_pMemoryManager->iRead(entityAddv + ofTeamId);
			if (entityAddv == uMyObject)
			{
				myTeamId = teamId;
			}
			if ((teamId == myTeamId) || (teamId < 1))
			{
				continue;
			}
			enemies++;

			position = g_pMemoryManager->v3Read(entityWorld + ofPosition);
			g_pViewMatrixManager->worldToScreen(position, positionOnScreen, &distanceFl, &distanceValue);
			if (positionOnScreen.z < 0.001f)
			{
				continue;
			}

			// Player Line
			isRobot = g_pMemoryManager->bRead(entityAddv + ofRobot);
			if (isRobot)
			{
				line[0].color = sf::Color::Green;
				line[1].color = sf::Color::Green;
			}
			else
			{
				line[0].color = sf::Color::Red;
				line[1].color = sf::Color::Red;
			}
			line[1].position = sf::Vector2f(positionOnScreen.x, positionOnScreen.y);
			g_pOverlay->window.draw(line, 2, sf::Lines);

			// Player Info
			playerNameAdd = g_pMemoryManager->dRead(entityAddv + ofPlayerNameAdd);
			thePlayerName = g_pMemoryManager->charp32Read(playerNameAdd);
			stateInt = g_pMemoryManager->iRead(entityAddv + ofStateInt);
			health = g_pMemoryManager->fRead(entityAddv + ofHealth);

			if (distanceValue > 200)
			{
				fixPosY = 45.f;
				headBoneSize = 1.f;
			}
			else if (distanceValue > 150)
			{
				fixPosY = 50.f;
				headBoneSize = 1.5f;
			}
			else if (distanceValue > 100)
			{
				fixPosY = 65.f;
				headBoneSize = 2.f;
			}
			else if (distanceValue > 50)
			{
				fixPosY = 70.f;
				headBoneSize = 2.5f;
			}
			else if (distanceValue > 40)
			{
				fixPosY = 100.f;
				headBoneSize = 3.f;
			}
			else if (distanceValue > 30)
			{
				fixPosY = 110.f;
				headBoneSize = 3.5f;
			}
			else if (distanceValue > 20)
			{
				fixPosY = 120.f;
				headBoneSize = 5.f;
			}
			else if (distanceValue > 10)
			{
				fixPosY = 150.f;
				headBoneSize = 10.f;
			}
			else
			{
				fixPosY = 200.f;
				headBoneSize = 15.f;
			}

			infoBg.setPosition(sf::Vector2f(positionOnScreen.x, positionOnScreen.y));
			infoBg.setOrigin(sf::Vector2f(101.f, fixPosY));
			g_pOverlay->window.draw(infoBg);

			playerName.setString(thePlayerName + "\n" + getState(stateInt));
			playerName.setPosition(sf::Vector2f(positionOnScreen.x, positionOnScreen.y));
			playerName.setOrigin(sf::Vector2f(-50.f + 101.f, -1.f + fixPosY));
			g_pOverlay->window.draw(playerName);

			healthBar.setPosition(sf::Vector2f(positionOnScreen.x, positionOnScreen.y));
			healthBar.setSize(sf::Vector2f((float) health * 2.f, 4.f));
			healthBar.setOrigin(sf::Vector2f(-1.f + 101.f, -31.f + fixPosY));
			g_pOverlay->window.draw(healthBar);

			healthBorder.setPosition(sf::Vector2f(positionOnScreen.x, positionOnScreen.y));
			healthBorder.setOrigin(sf::Vector2f(-1.f + 101.f, -31.f + fixPosY));
			g_pOverlay->window.draw(healthBorder);

			playerInfo.setString(to_string(distanceValue) + "\nm");
			playerInfo.setPosition(sf::Vector2f(positionOnScreen.x, positionOnScreen.y));
			playerInfo.setOrigin(sf::Vector2f(-1.f + 101.f, -1.f + fixPosY));
			g_pOverlay->window.draw(playerInfo);

			playerInfo.setString(to_string(health) + "\nhp");
			playerInfo.setOrigin(sf::Vector2f(-180.f + 101.f, -1.f + fixPosY));
			g_pOverlay->window.draw(playerInfo);

			meshSkeleton = g_pMemoryManager->dRead(entityAddv + ofMeshSkeleton);
			actor = g_pMemoryManager->structRead(meshSkeleton + ofActor);
			componentToWorldMatrix = ToMatrixWithScale(actor.Translation, actor.Scale3D, actor.Rotation);
			boneAddv = g_pMemoryManager->dRead(meshSkeleton + ofBoneAddv);

			bonePosition = getBonePos(6); // HEAD
			playerHead.setRadius(headBoneSize);
			playerHead.setPosition(bonePosition.x, bonePosition.y);
			playerHead.setOrigin(headBoneSize, (headBoneSize * 2) + 1.f);
			g_pOverlay->window.draw(playerHead);

			bonePosition = getBonePos(63); // right hand
			armsLegs[0].position = sf::Vector2f(bonePosition.x, bonePosition.y);
			bonePosition = getBonePos(34); // right wrest
			armsLegs[1].position = sf::Vector2f(bonePosition.x, bonePosition.y);
			bonePosition = getBonePos(33); // right shoulder
			armsLegs[2].position = sf::Vector2f(bonePosition.x, bonePosition.y);
			bonePosition = getBonePos(6); // head
			armsLegs[3].position = sf::Vector2f(bonePosition.x, bonePosition.y);
			bonePosition = getBonePos(12); // left shoulder
			armsLegs[4].position = sf::Vector2f(bonePosition.x, bonePosition.y);
			bonePosition = getBonePos(13); // left wrest
			armsLegs[5].position = sf::Vector2f(bonePosition.x, bonePosition.y);
			bonePosition = getBonePos(64); // left hand
			armsLegs[6].position = sf::Vector2f(bonePosition.x, bonePosition.y);
			g_pOverlay->window.draw(armsLegs);

			bonePosition = getBonePos(59); // right foot
			armsLegs[0].position = sf::Vector2f(bonePosition.x, bonePosition.y);
			bonePosition = getBonePos(58); // right knee
			armsLegs[1].position = sf::Vector2f(bonePosition.x, bonePosition.y);
			bonePosition = getBonePos(57); // right waste
			armsLegs[2].position = sf::Vector2f(bonePosition.x, bonePosition.y);
			bonePosition = getBonePos(2); // body middle
			armsLegs[3].position = sf::Vector2f(bonePosition.x, bonePosition.y);
			bonePosition = getBonePos(53); // left waste
			armsLegs[4].position = sf::Vector2f(bonePosition.x, bonePosition.y);
			bonePosition = getBonePos(54); // left knee
			armsLegs[5].position = sf::Vector2f(bonePosition.x, bonePosition.y);
			bonePosition = getBonePos(55); // left foot
			armsLegs[6].position = sf::Vector2f(bonePosition.x, bonePosition.y);
			g_pOverlay->window.draw(armsLegs);

			bonePosition = getBonePos(6); // head
			body[0].position = sf::Vector2f(bonePosition.x, bonePosition.y);
			//bonePosition = getBonePos(); // 
			body[1].position = sf::Vector2f(bonePosition.x, bonePosition.y);
			bonePosition = getBonePos(5); // 
			body[2].position = sf::Vector2f(bonePosition.x, bonePosition.y);
			//bonePosition = getBonePos(); // 
			body[3].position = sf::Vector2f(bonePosition.x, bonePosition.y);
			bonePosition = getBonePos(2); // 
			body[4].position = sf::Vector2f(bonePosition.x, bonePosition.y);
			g_pOverlay->window.draw(body);

		}

		if (enemies > 0)
		{
			if (playAlert)
			{
				alert.openFromFile("./PUBGAlert.wav");
				alert.setVolume(80);
				alert.play();
				playAlert = false;
			}

			itemName.setString("There are ( " + to_string(enemies) + " ) enemies around!");
			itemName.setFillColor(sf::Color(255, 164, 0, 255));
			itemName.setPosition(sf::Vector2f(float (g_pOverlay->width) / 2.f, 50.f));
			sf::FloatRect itemBounds;
			itemBounds = itemName.getGlobalBounds();
			itemName.setOrigin(sf::Vector2f(itemBounds.width / 2, 10.f));
			itemBounds = itemName.getGlobalBounds();
			itemBg.setPosition(itemBounds.left, itemBounds.top);
			itemBg.setSize(sf::Vector2f(itemBounds.width + 10.f, itemBounds.height + 10.f));
			g_pOverlay->window.draw(itemBg);
			g_pOverlay->window.draw(itemName);
		}

	}

	if (enemies == 0)
	{
		playAlert = true;
	}

}

string ESP::replace(string& str, const string& from)
{
	size_t start_pos = str.find(from);
	if (start_pos == string::npos)
		return str;
	str.replace(start_pos, from.length(), "");
	return str;
}

string ESP::getState(int state)
{
	switch (state)
	{
	case 0:
		return "Jump of Baot";
	case 1:
		return "In Water";
	case 8:
		return "Standing";
	case 9:
		return "Walking";
	case 10:
		return "Landing";
	case 11:
		return "Running";
	case 16:
		return "Crouching";
	case 17:
		return "Crouching Walk";
	case 19:
		return "Crouching Run";
	case 32:
		return "Snaking";
	case 33:
		return "Snaking Walk";
	case 35:
		return "Snaking Run";
	case 5445:
	case 762:
		return "Snake";
	case 72:
	case 73:
	case 75:
		return "Jumping";
	case 264:
	case 272:
	case 273:
	case 288:
	case 265:
	case 329:
		return "Relaoding";
	case 136:
	case 137:
	case 144:
	case 145:
	case 160:
	case 648:
	case 649:
	case 1160:
	case 1161:
	case 1169:
	case 201:
		return "Firing";
	case 4194304:
	case 4194307:
		return "Swimming";
	case 131070:
	case 131071:
	case 131072:
	case 131073:
	case 131074:
	case 131075:
		return "Knocked";
	case 524296:
		return "Driving Vehicle";
	case 524288:
		return "Driving Boat";
	case 1048584:
		return "Set in Vehicle";
	case 1048576:
		return "Set in Boat";
	case 33554440:
		return "Set in Plane";
	case 3146248:
		return "Aim in vehicle";
	case 3146240:
		return "Aim in boat";
	case 3145736:
		return "Beek in vehicle";
	case 3145728:
		return "Beek in boat";
	case 1050632:
		return "Change vehicle seat";
	case 1050624:
		return "Change boat seat";
	case 32784:
		return "Reviving";
	case 16392:
	case 16393:
	case 16401:
	case 16416:
	case 16417:
	case 16457:
	case 16400:
	case 17401:
	case 17417:
	case 17424:
	case 17425:
		return "Throwing Bomp";
	case 16777224:
		return "Climping";
	case 8200:
	case 8208:
		return "Punish, Stab";
	case 520:
	case 544:
	case 521:
	case 656:
	case 528:
		return "Aiming";
	case 1680:
	case 1672:
	case 1673:
	case 1032:
	case 1544:
	case 1545:
	case 1033:
		return "Peeking";
	case 2056:
		return "Change Weapon";
	case 65544:
		return "Healing";
	case 65545:
		return "Healing & Walk";
	case 268435464:
		return "Playing Emotion";
	case 4194305:
		return "Diving, Floating";
	}

	return to_string(state);
}


D3DMatrix ESP::ToMatrixWithScale(Vector3f translation, Vector3f scale, Vector4f rot)
{
	D3DMatrix m;
	m._41 = translation.x;
	m._42 = translation.y;
	m._43 = translation.z;

	float x2 = rot.x + rot.x;
	float y2 = rot.y + rot.y;
	float z2 = rot.z + rot.z;

	float xx2 = rot.x * x2;
	float yy2 = rot.y * y2;
	float zz2 = rot.z * z2;
	m._11 = (1.0f - (yy2 + zz2)) * scale.x;
	m._22 = (1.0f - (xx2 + zz2)) * scale.y;
	m._33 = (1.0f - (xx2 + yy2)) * scale.z;

	float yz2 = rot.y * z2;
	float wx2 = rot.w * x2;
	m._32 = (yz2 - wx2) * scale.z;
	m._23 = (yz2 + wx2) * scale.y;

	float xy2 = rot.x * y2;
	float wz2 = rot.w * z2;
	m._21 = (xy2 - wz2) * scale.y;
	m._12 = (xy2 + wz2) * scale.x;

	float xz2 = rot.x * z2;
	float wy2 = rot.w * y2;
	m._31 = (xz2 + wy2) * scale.z;
	m._13 = (xz2 - wy2) * scale.x;

	m._14 = 0.0f;
	m._24 = 0.0f;
	m._34 = 0.0f;
	m._44 = 1.0f;

	return m;
}

Vector3f ESP::getBonePos(int boneNum)
{
	bone = g_pMemoryManager->structRead(boneAddv + (boneNum * 48));
	boneMatrix = ToMatrixWithScale(bone.Translation, bone.Scale3D, bone.Rotation);
	newMatrix =
	{
		boneMatrix._11 * componentToWorldMatrix._11 + boneMatrix._12 * componentToWorldMatrix._21 + boneMatrix._13 * componentToWorldMatrix._31 + boneMatrix._14 * componentToWorldMatrix._41,
		boneMatrix._11 * componentToWorldMatrix._12 + boneMatrix._12 * componentToWorldMatrix._22 + boneMatrix._13 * componentToWorldMatrix._32 + boneMatrix._14 * componentToWorldMatrix._42,
		boneMatrix._11 * componentToWorldMatrix._13 + boneMatrix._12 * componentToWorldMatrix._23 + boneMatrix._13 * componentToWorldMatrix._33 + boneMatrix._14 * componentToWorldMatrix._43,
		boneMatrix._11 * componentToWorldMatrix._14 + boneMatrix._12 * componentToWorldMatrix._24 + boneMatrix._13 * componentToWorldMatrix._34 + boneMatrix._14 * componentToWorldMatrix._44,
		boneMatrix._21 * componentToWorldMatrix._11 + boneMatrix._22 * componentToWorldMatrix._21 + boneMatrix._23 * componentToWorldMatrix._31 + boneMatrix._24 * componentToWorldMatrix._41,
		boneMatrix._21 * componentToWorldMatrix._12 + boneMatrix._22 * componentToWorldMatrix._22 + boneMatrix._23 * componentToWorldMatrix._32 + boneMatrix._24 * componentToWorldMatrix._42,
		boneMatrix._21 * componentToWorldMatrix._13 + boneMatrix._22 * componentToWorldMatrix._23 + boneMatrix._23 * componentToWorldMatrix._33 + boneMatrix._24 * componentToWorldMatrix._43,
		boneMatrix._21 * componentToWorldMatrix._14 + boneMatrix._22 * componentToWorldMatrix._24 + boneMatrix._23 * componentToWorldMatrix._34 + boneMatrix._24 * componentToWorldMatrix._44,
		boneMatrix._31 * componentToWorldMatrix._11 + boneMatrix._32 * componentToWorldMatrix._21 + boneMatrix._33 * componentToWorldMatrix._31 + boneMatrix._34 * componentToWorldMatrix._41,
		boneMatrix._31 * componentToWorldMatrix._12 + boneMatrix._32 * componentToWorldMatrix._22 + boneMatrix._33 * componentToWorldMatrix._32 + boneMatrix._34 * componentToWorldMatrix._42,
		boneMatrix._31 * componentToWorldMatrix._13 + boneMatrix._32 * componentToWorldMatrix._23 + boneMatrix._33 * componentToWorldMatrix._33 + boneMatrix._34 * componentToWorldMatrix._43,
		boneMatrix._31 * componentToWorldMatrix._14 + boneMatrix._32 * componentToWorldMatrix._24 + boneMatrix._33 * componentToWorldMatrix._34 + boneMatrix._34 * componentToWorldMatrix._44,
		boneMatrix._41 * componentToWorldMatrix._11 + boneMatrix._42 * componentToWorldMatrix._21 + boneMatrix._43 * componentToWorldMatrix._31 + boneMatrix._44 * componentToWorldMatrix._41,
		boneMatrix._41 * componentToWorldMatrix._12 + boneMatrix._42 * componentToWorldMatrix._22 + boneMatrix._43 * componentToWorldMatrix._32 + boneMatrix._44 * componentToWorldMatrix._42,
		boneMatrix._41 * componentToWorldMatrix._13 + boneMatrix._42 * componentToWorldMatrix._23 + boneMatrix._43 * componentToWorldMatrix._33 + boneMatrix._44 * componentToWorldMatrix._43,
		boneMatrix._41 * componentToWorldMatrix._14 + boneMatrix._42 * componentToWorldMatrix._24 + boneMatrix._43 * componentToWorldMatrix._34 + boneMatrix._44 * componentToWorldMatrix._44
	};
	Vector3f bonePos;
	bonePos.x = newMatrix._41;
	bonePos.y = newMatrix._42;
	bonePos.z = newMatrix._43;
	Vector3f bonePosOnScreen;
	g_pViewMatrixManager->worldBoneToScreen(bonePos, bonePosOnScreen);
	return bonePosOnScreen;
}
