#include "PlayScene.h"
#include"SimpleAudioEngine.h"
#include "GameOverScene.h"
#include"GameDefine.h"
#include"Gamepause.h"
#include"cocos2d.h"

bool poison;
int _numCollected;
int m_time;
int judge;

USING_NS_CC;

using namespace CocosDenshion;
using namespace ui;

PlaySceneCover *PlayScene::_cover = NULL;

Scene* PlayScene1::createScene()
{
	auto scene = Scene::create();
	auto layer = PlayScene1::create();
	auto cover = PlaySceneCover::create();
	_cover = cover;
	_cover->setGameLayer(layer);

	scene->addChild(layer);
	scene->addChild(cover);
	return scene;
}

Scene* PlayScene2::createScene()
{
	auto scene = Scene::create();

	auto layer = PlayScene2::create();
	scene->addChild(layer);

	return scene;
}

Scene* PlayScene3::createScene()
{
	auto scene = Scene::create();

	auto layer = PlayScene3::create();
	scene->addChild(layer);

	return scene;
}

// on "init" you need to initialize your instance
bool PlayScene::init()
{
	poison = false;
	 _numCollected = 0;
	m_time = 180;
	judge = 0;
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	SimpleAudioEngine::getInstance()->preloadEffect("pickup.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("hit.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("move.wav");

	//ontouch
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch *touch, Event *unused_event)->bool { return true; };
	listener->onTouchEnded = CC_CALLBACK_2(PlayScene::onTouchEnded, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//keyboard
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	EventListenerKeyboard* keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(PlayScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(PlayScene::onKeyReleased, this);
	dispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	//schedule
	scheduleUpdate();

	schedule(schedule_selector(PlayScene::bullet), 1.5f);
	schedule(schedule_selector(PlayScene::myusingtime), 1.0f);
	schedule(schedule_selector(PlayScene::myhelper));

	schedule(schedule_selector(PlayScene::unchanged_test));
	schedule(schedule_selector(PlayScene::test_guard));
	schedule(schedule_selector(PlayScene::test_super));
	schedule(schedule_selector(PlayScene::testCollisions));

	return true;
}

bool PlayScene1::init()
{
	if (!Layer::init())
	{
		return false;
	}
	_tileMap = TMXTiledMap::create("MyMap.tmx");
	auto _background_1 = _tileMap->getLayer("Background_1");
	auto _background_2 = _tileMap->getLayer("Background_2");

	_foreground_1 = _tileMap->getLayer("Foreground_1");
	_foreground_2 = _tileMap->getLayer("Foreground_2");
	_foreground_3 = _tileMap->getLayer("Foreground_3");
	_foreground_4 = _tileMap->getLayer("Foreground_4");
	_foreground_5 = _tileMap->getLayer("Foreground_5");
	_foreground_6 = _tileMap->getLayer("Foreground_6");

	_meta = _tileMap->getLayer("Meta");
	_meta->setVisible(false);

	//get objects
	TMXObjectGroup *objects = _tileMap->getObjectGroup("Objects");
	CCASSERT(NULL != objects, "'Objects' object group not found");
	auto spawnPoint = objects->getObject("SpawnPoint");
	CCASSERT(!spawnPoint.empty(), "SpawnPoint object not found");
	int x = spawnPoint["x"].asInt();
	int y = spawnPoint["y"].asInt();

	_player = Sprite::create("Hero.png");
	_player->setPosition(x, y);
	addChild(_player);

	ViewPoint(_player->getPosition());

	addChild(_tileMap, -1);

	for (auto& eSpawnPoint : objects->getObjects()) {
		ValueMap& dict = eSpawnPoint.asValueMap();
		int i = dict["Sequence"].asInt();
		if (dict["Enemy_0"].asInt() == 1) {
			x = dict["x"].asInt();
			y = dict["y"].asInt();
			this->addEnemy(Point(x, y));
		}
		else if (dict["Enemy_Move"].asInt() == 1) {
			x = dict["x"].asInt();
			y = dict["y"].asInt();
			this->Enemy_Move(Point(x, y), i);
		}
		else if (dict["Enemy_Stable"].asInt() == 1) {
			x = dict["x"].asInt();
			y = dict["y"].asInt();
			this->Enemy_Stable(Point(x, y));
		}
	}
	group_2 = true;
	PlayScene::init();
}

//有待后续开发
bool PlayScene2::init()
{
	if (!Layer::init())
	{
		return false;
	}
	PlayScene::init();
}
bool PlayScene3::init()
{
	if (!Layer::init())
	{
		return false;
	}
	PlayScene::init();
}

void PlayScene::PlayerPosition(Point position)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	TMXObjectGroup *objects = _tileMap->getObjectGroup("Objects");
	CCASSERT(NULL != objects, "'Objects' object group not found");
	auto spawnPoint = objects->getObject("SpawnPoint");
	CCASSERT(!spawnPoint.empty(), "SpawnPoint object not found");
	int x = spawnPoint["x"].asInt();
	int y = spawnPoint["y"].asInt();

	//enemy_1    enemy_2    appearance
	for (auto& eSpawnPoint : objects->getObjects()) {
		ValueMap& dict = eSpawnPoint.asValueMap();
		if ((dict["Enemy_1"].asInt() == 1) && (_numCollected == 6||_numCollected == 7) && group_2 == true){
			x = dict["x"].asInt();
			y = dict["y"].asInt();
			this->addEnemy(Point(x, y));
			done_2 = true;
		}
	}
	if (done_2 == true)
		group_2 = false;

	for (auto& eSpawnPoint : objects->getObjects()) {
		ValueMap& dict = eSpawnPoint.asValueMap();
		if ((dict["Enemy_2"].asInt() == 1) && (_numCollected == 10||_numCollected == 11) && group_3 == true){
			x = dict["x"].asInt();
			y = dict["y"].asInt();
			this->addEnemy_2(Point(x, y));
			done_3 = true;
		}
	}
	if (done_3 == true)
		group_3 = false;

	if (move_guard == true && guard_done == false){
		guard_done = true;
		for (Sprite* target : stable_enemies){
			moveEnemy(target);
		}
	}

	Point tileCoord = this->tileToPosition(position);
	int tileGid = _meta->getTileGIDAt(tileCoord);
	if (tileGid) {
		auto properties = _tileMap->getPropertiesForGID(tileGid).asValueMap();
		if (!properties.empty()) {
			auto collision = properties["Collidable"].asString();
			if (collision == "True"&&get_key == false) {
				SimpleAudioEngine::getInstance()->playEffect("hit.wav");
				return;
			}
			auto collectable = properties["Collectable"].asString();
			auto key = properties["Key"].asString();
			auto poisoning = properties["Poison"].asString();
			auto special = properties["Special"].asString();
			auto guard = properties["Guard"].asString();
			auto lose = properties["Lose"].asString();
			auto in = properties["In"].asString();
			auto out = properties["Out"].asString();
			auto help = properties["Help"].asString();
			auto bullet = properties["Bullet"].asString();

			auto pos = _player->getPosition();
			TTFConfig config("fonts/haibaoti.ttf", 30);
			Label *_label;

			if (collectable == "True") {
				_meta->removeTileAt(tileCoord);
				_foreground_1->removeTileAt(tileCoord);
				_foreground_3->removeTileAt(tileCoord);

				_numCollected++;

				SimpleAudioEngine::getInstance()->playEffect("pickup.wav");
				
				_label = Label::createWithTTF(config, StringUtils::format("SCORE+1"));
				_label->setPosition(pos);
				_label->setColor(Color3B::RED);
				_label->runAction(Sequence::create(DelayTime::create(0.5), FadeOut::create(0.5), RemoveSelf::create(true), NULL));
				addChild(_label);

			}
			else if (key == "True"){
				_meta->removeTileAt(tileCoord);
				_foreground_6->removeTileAt(tileCoord);
				get_key = true;
				
				_label = Label::createWithTTF(config, StringUtils::format("GET A KEY"));
				_label->setPosition(pos);
				_label->runAction(Sequence::create(DelayTime::create(5), FadeOut::create(1), RemoveSelf::create(true), NULL));
				addChild(_label);
				SimpleAudioEngine::getInstance()->playEffect("pickup.wav");
			}
			else if (guard == "True"){
				_meta->removeTileAt(tileCoord);
				_foreground_1->removeTileAt(tileCoord);
				move_guard = true;
			}
			else if (poisoning == "True"){
				_meta->removeTileAt(tileCoord);
				_foreground_1->removeTileAt(tileCoord);
				poison = true;
				_numCollected--;
				SimpleAudioEngine::getInstance()->playEffect("poison.wav");
				
				 _label = Label::createWithTTF(config, StringUtils::format("SCORE-1"));
				_label->setPosition(pos);
				_label->setColor(Color3B::BLACK);
				_label->runAction(Sequence::create(DelayTime::create(0.5), FadeOut::create(0.5), RemoveSelf::create(true), NULL));
				addChild(_label);
			}
			else if (special == "True"){
				_numCollected += 2;
				_meta->removeTileAt(tileCoord);
				_foreground_1->removeTileAt(tileCoord);
				speedup = true;

				SimpleAudioEngine::getInstance()->playEffect("pickup.wav");
				
				_label = Label::createWithTTF(config, StringUtils::format("SCORE+2"));
				_label->setPosition(pos);
				_label->setColor(Color3B::RED);
				_label->runAction(Sequence::create(DelayTime::create(0.5), FadeOut::create(0.5), RemoveSelf::create(true), NULL));
				addChild(_label);

			}
			else if (lose == "True")
			{
				SimpleAudioEngine::getInstance()->playEffect("lose.mp3");
				this->lose();
			}
			else if (in == "True"){
				auto move_by = MoveBy::create(1.0f, Vec2(128, 0));
				_player->runAction(move_by);
			}
			else if (out == "True"){
				auto move_by = MoveBy::create(1.0f, Vec2(-192, 0));
				_player->runAction(move_by);
			}
			else if (help == "True"){
				get_helper = true;
				special_pos = position;
				_meta->removeTileAt(tileCoord);
				_foreground_1->removeTileAt(tileCoord);
				SimpleAudioEngine::getInstance()->playEffect("pickup.wav");
			}
			else if (bullet == "True"){
				get_bullet = true;
				_meta->removeTileAt(tileCoord);
				_foreground_3->removeTileAt(tileCoord);

				SimpleAudioEngine::getInstance()->playEffect("pickup.wav");
				
				_label = Label::createWithTTF(config, StringUtils::format("ARMED"));
				_label->setColor(Color3B::BLUE);
				_label->setPosition(pos);
				_label->runAction(Sequence::create(DelayTime::create(2), FadeOut::create(1), RemoveSelf::create(true), NULL));
				addChild(_label);
			}
		}
	}
	SimpleAudioEngine::getInstance()->playEffect("move.wav");
	_player->setPosition(position);
}

void PlayScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	auto playerPos = _player->getPosition();

	if (EventKeyboard::KeyCode::KEY_W == keyCode)
	{
		playerPos.y += _tileMap->getTileSize().height;
	}
	if (EventKeyboard::KeyCode::KEY_S == keyCode)
	{
		playerPos.y -= _tileMap->getTileSize().height;
	}
	if (EventKeyboard::KeyCode::KEY_A == keyCode)
	{
		playerPos.x -= _tileMap->getTileSize().width;
	}
	if (EventKeyboard::KeyCode::KEY_D == keyCode)
	{
		playerPos.x += _tileMap->getTileSize().width;

	}
	if (playerPos.x <= (_tileMap->getMapSize().width * _tileMap->getMapSize().width) &&
		playerPos.y <= (_tileMap->getMapSize().height * _tileMap->getMapSize().height) &&
		playerPos.y >= 0 &&
		playerPos.x >= 0)
	{
		this->PlayerPosition(playerPos);
	}

	this->ViewPoint(_player->getPosition());
}

void PlayScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
}

void PlayScene::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (judge == 0 && get_bullet == true) {
		judge = 1;
		// Find where the touch is
		auto touchLocation = touch->getLocation();
		touchLocation = this->convertToNodeSpace(touchLocation);

		// Create a projectile and put it at the player's location
		auto projectile = Sprite::create("bullet.png");
		projectile->setPosition(_player->getPosition());
		this->addChild(projectile);

		int realX;

		// Are we shooting to the left or right?
		auto diff = touchLocation - _player->getPosition();
		if (diff.x > 0)
		{
			realX = (_tileMap->getMapSize().width * _tileMap->getTileSize().width) +
				(projectile->getContentSize().width / 2);
		}
		else {
			realX = -(_tileMap->getMapSize().width * _tileMap->getTileSize().width) -
				(projectile->getContentSize().width / 2);
		}
		float ratio = (float)diff.y / (float)diff.x;
		int realY = ((realX - projectile->getPosition().x) * ratio) + projectile->getPosition().y;
		auto realDest = Point(realX, realY);

		// Determine the length of how far we're shooting
		int offRealX = realX - projectile->getPosition().x;
		int offRealY = realY - projectile->getPosition().y;
		float length = sqrtf((offRealX*offRealX) + (offRealY*offRealY));
		float velocity;
		if (speedup == false)
			velocity = 60 / 1; 
		else
			velocity = 100 / 1;
		float realMoveDuration = length / velocity;

		SimpleAudioEngine::getInstance()->playEffect("bullet.wav");
		// Move projectile to actual endpoint
		auto actionMoveDone = CallFuncN::create(CC_CALLBACK_1(PlayScene::projectileMoveFinished, this));
		projectile->runAction(Sequence::create(MoveTo::create(realMoveDuration, realDest), actionMoveDone, NULL));

		_projectiles.pushBack(projectile);
	}
}

void PlayScene::ViewPoint(Point position) {
	auto winSize = Director::getInstance()->getWinSize();

	//get actual position
	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height / 2);
	auto actualPosition = Point(x, y);

	auto centerOfView = Point(winSize.width / 2, winSize.height / 2);
	auto viewPoint = centerOfView - actualPosition;
	this->setPosition(viewPoint);
}

Point PlayScene::tileToPosition(Point position)
{
	int x = position.x / _tileMap->getTileSize().width;
	int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - position.y) / _tileMap->getTileSize().height;
	return Point(x, y);
}

void PlayScene::Enemy_Stable(Point pos)
{
	auto enemy = Sprite::create("enemy_stable.png");
	enemy->setPosition(pos);
	enemy->setOpacity(120);
	addChild(enemy);
	stable_enemies.pushBack(enemy);
}

void PlayScene::test_guard(float dt)
{
	//test if the enemy hit the hero;if so,the hero will lose a HP
	for (Sprite *target : stable_enemies) {
		auto targetRect = Rect(
			target->getPosition().x - (target->getContentSize().width / 2),
			target->getPosition().y - (target->getContentSize().height / 2),
			target->getContentSize().width,
			target->getContentSize().height);
		if (targetRect.containsPoint(_player->getPosition())) {
			if (heroHp > 0 && target->getOpacity() != 250)
			{
				SimpleAudioEngine::getInstance()->playEffect("hurt.wav");
				heroHp--;
				target->setOpacity(250);
				auto pos = _player->getPosition();
				TTFConfig config("fonts/haibaoti.ttf", 30);
				auto _label = Label::createWithTTF(config, StringUtils::format("HP-1"));
				_label->setPosition(pos);
				_label->setColor(Color3B::RED);
				_label->runAction(Sequence::create(DelayTime::create(0.5), FadeOut::create(0.5), RemoveSelf::create(true), NULL));
				addChild(_label);
			}
			else if (heroHp <= 0)
			{
				SimpleAudioEngine::getInstance()->playEffect("lose.mp3");
				this->lose();
			}
		}
	}

	Vector<Sprite*> projectilesToDelete;
	for (Sprite *projectile : _projectiles) {
		auto projectileRect = Rect(
			projectile->getPositionX() - projectile->getContentSize().width / 2,
			projectile->getPositionY() - projectile->getContentSize().height / 2,
			projectile->getContentSize().width,
			projectile->getContentSize().height);

		Vector<Sprite*> targetsToDelete;
		// to see if any projectile hit
		for (Sprite *target : stable_enemies) {
			auto targetRect = Rect(
				target->getPositionX() - target->getContentSize().width / 2,
				target->getPositionY() - target->getContentSize().height / 2,
				target->getContentSize().width,
				target->getContentSize().height);

			if (projectileRect.intersectsRect(targetRect)) {
				targetsToDelete.pushBack(target);
			}
		}
		// delete all hit enemies
		for (Sprite *target : targetsToDelete) {
			stable_enemies.eraseObject(target);
			this->removeChild(target);
		}

		if (targetsToDelete.size() > 0) {
			projectilesToDelete.pushBack(projectile);
		}
		targetsToDelete.clear();
	}
	// remove projectiles
	for (Sprite *projectile : projectilesToDelete) {
		_projectiles.eraseObject(projectile);
		this->removeChild(projectile);
	}
	projectilesToDelete.clear();
}

void PlayScene::Enemy_Move(Point pos, int i)
{
	//enemy for four direction
	auto enemy = Sprite::create("enemy_move.png");
	enemy->setPosition(pos);
	addChild(enemy);
	if (i == 1){
		auto moveBy_1 = MoveBy::create(1.5f, Vec2(-160, 0));
		auto moveBy_2 = MoveBy::create(1.5f, Vec2(160, 0));
		auto seq = Sequence::create(moveBy_1, moveBy_2, nullptr);
		auto repeat = RepeatForever::create(seq);

		enemy->runAction(repeat);
	}
	else if (i == 2){
		auto moveBy_1 = MoveBy::create(1.5f, Vec2(160, 0));
		auto moveBy_2 = MoveBy::create(1.5f, Vec2(-160, 0));
		auto seq = Sequence::create(moveBy_1, moveBy_2, nullptr);
		auto repeat = RepeatForever::create(seq);

		enemy->runAction(repeat);
	}
	else if (i == 3){
		auto moveBy_1 = MoveBy::create(1.5f, Vec2(0, -96));
		auto moveBy_2 = MoveBy::create(1.5f, Vec2(0, 96));
		auto seq = Sequence::create(moveBy_1, moveBy_2, nullptr);
		auto repeat = RepeatForever::create(seq);

		enemy->runAction(repeat);
	}
	else if (i == 4){
		auto moveBy_1 = MoveBy::create(1.5f, Vec2(0, 96));
		auto moveBy_2 = MoveBy::create(1.5f, Vec2(0, -96));
		auto seq = Sequence::create(moveBy_1, moveBy_2, nullptr);
		auto repeat = RepeatForever::create(seq);

		enemy->runAction(repeat);
	}
	unchanged_enemies.pushBack(enemy);
}

void PlayScene::addEnemy(Point pos)
{
	pos.x+=100 * rand_minus1_1();
	pos.y+=100 * rand_minus1_1();
	auto enemy = Sprite::create("enemy_0.png");
	enemy->setPosition(pos);
	this->addChild(enemy);
	this->moveEnemy(enemy);

	_enemies.pushBack(enemy);

}

void PlayScene::addEnemy_2(Point pos)  
{
	pos.x += 100 * rand_minus1_1();
	pos.y += 100 * rand_minus1_1();   
	auto enemy = Sprite::create("enemy_2.png");
	enemy->setPosition(pos);
	this->addChild(enemy);
	this->moveEnemy(enemy);

	_enemies_2.pushBack(enemy);
}

void PlayScene::enemyMoveFinished(Object *pSender)
{
	Sprite *enemy = (Sprite *)pSender;

	this->moveEnemy(enemy);
}

void PlayScene::moveEnemy(Sprite *enemy)
{
	//speed
	actualDuration = 0.25f;
	if (poison == true)
		actualDuration = 0.15f;

	// Create the actions
	auto position = (_player->getPosition() - enemy->getPosition()).getNormalized() * 10;
	auto actionMove = MoveBy::create(actualDuration, position);
	auto actionMoveDone = CallFuncN::create(CC_CALLBACK_1(PlayScene::enemyMoveFinished, this));
	enemy->runAction(Sequence::create(actionMove, actionMoveDone, NULL));
}

void PlayScene::projectileMoveFinished(Object *pSender)
{
	judge = 0;
	Sprite *sprite = (Sprite *)pSender;
	this->removeChild(sprite);
	_projectiles.eraseObject(sprite);
}

void PlayScene::unchanged_test(float dt)
{
	for (Sprite *target : unchanged_enemies) {
		auto targetRect = Rect(
			target->getPosition().x - (target->getContentSize().width / 2),
			target->getPosition().y - (target->getContentSize().height / 2),
			target->getContentSize().width,
			target->getContentSize().height);
		if (targetRect.containsPoint(_player->getPosition())) {
				SimpleAudioEngine::getInstance()->playEffect("lose.mp3");
				this->lose();
			}
		}
}

void PlayScene::testCollisions(float dt)
{
	for (Sprite *target : _enemies) {
		auto targetRect = Rect(
			target->getPosition().x - (target->getContentSize().width / 2),
			target->getPosition().y - (target->getContentSize().height / 2),
			target->getContentSize().width,
			target->getContentSize().height);
		if (targetRect.containsPoint(_player->getPosition())) {

			if (heroHp > 0 && target->getOpacity() != 250)
			{
				SimpleAudioEngine::getInstance()->playEffect("hurt.wav");
				heroHp--;
				target->setOpacity(250);
				auto pos = _player->getPosition();
				TTFConfig config("fonts/haibaoti.ttf", 30);
				auto _label = Label::createWithTTF(config, StringUtils::format("HP-1"));
				_label->setPosition(pos);
				_label->setColor(Color3B::RED);
				_label->runAction(Sequence::create(DelayTime::create(0.5), FadeOut::create(0.5), RemoveSelf::create(true), NULL));
				addChild(_label);
			}
			else if (heroHp <= 0)
			{
				SimpleAudioEngine::getInstance()->playEffect("lose.mp3");
				this->lose();
			}

		}
	}

	Vector<Sprite*> projectilesToDelete;
	for (Sprite *projectile : _projectiles) {
		auto projectileRect = Rect(
			projectile->getPositionX() - projectile->getContentSize().width / 2,
			projectile->getPositionY() - projectile->getContentSize().height / 2,
			projectile->getContentSize().width,
			projectile->getContentSize().height);

		Vector<Sprite*> targetsToDelete;
		//to see if any projectile hit
		for (Sprite *target : _enemies) {
			auto targetRect = Rect(
				target->getPositionX() - target->getContentSize().width / 2,
				target->getPositionY() - target->getContentSize().height / 2,
				target->getContentSize().width,
				target->getContentSize().height);

			if (projectileRect.intersectsRect(targetRect)){
				targetsToDelete.pushBack(target);
			}
		}
		// delete all hit enemies
		for (Sprite *target : targetsToDelete) {
			_enemies.eraseObject(target);
			this->removeChild(target);
			auto pos = target->getPosition();

			auto explosion = ParticleExplosion::create();
			explosion->setTexture(Director::getInstance()->getTextureCache()->addImage("fire.png"));
			explosion->setPosition(pos);

			explosion->setTotalParticles(500);
			explosion->setLife(0.6f);

			this->addChild(explosion);
		}
		if (targetsToDelete.size() > 0) {
			projectilesToDelete.pushBack(projectile);
		}
		targetsToDelete.clear();
	}
	// remove projectiles
	for (Sprite *projectile : projectilesToDelete) {
		_projectiles.eraseObject(projectile);
		this->removeChild(projectile);
	}
	projectilesToDelete.clear();
}

void PlayScene::test_super(float dt)
{
	for (Sprite *target : _enemies_2) {
		auto targetRect = Rect(
			target->getPosition().x - (target->getContentSize().width / 2),
			target->getPosition().y - (target->getContentSize().height / 2),
			target->getContentSize().width,
			target->getContentSize().height);
		if (targetRect.containsPoint(_player->getPosition())) {
			if (heroHp > 0 && target->getOpacity() != 250)
			{
				SimpleAudioEngine::getInstance()->playEffect("hurt.wav");
				heroHp--;
				target->setOpacity(250);
				auto pos = _player->getPosition();
				TTFConfig config("fonts/haibaoti.ttf", 30);
				auto _label = Label::createWithTTF(config, StringUtils::format("HP-1"));
				_label->setPosition(pos);
				_label->setColor(Color3B::RED);
				_label->runAction(Sequence::create(DelayTime::create(0.5), FadeOut::create(0.5), RemoveSelf::create(true), NULL));
				addChild(_label);
			}
			else if (heroHp <= 0)
			{
				SimpleAudioEngine::getInstance()->playEffect("lose.mp3");
				this->lose();
			}
		}
	}

	Vector<Sprite*> projectilesToDelete;
	for (Sprite *projectile : _projectiles) {
		auto projectileRect = Rect(
			projectile->getPositionX() - projectile->getContentSize().width / 2,
			projectile->getPositionY() - projectile->getContentSize().height / 2,
			projectile->getContentSize().width,
			projectile->getContentSize().height);

		Vector<Sprite*> targetsToDelete;
		//to see if any projectile hit
		for (Sprite *target : _enemies_2) {
			int hp;
			auto targetRect = Rect(
				target->getPositionX() - target->getContentSize().width / 2,
				target->getPositionY() - target->getContentSize().height / 2,
				target->getContentSize().width,
				target->getContentSize().height);
			//to hit the enemy_2
			if (projectileRect.intersectsRect(targetRect)){
				hp = target->getOpacity();
				target->setOpacity(hp / 2);
				projectilesToDelete.pushBack(projectile);
				if (target->getOpacity()<50){
					targetsToDelete.pushBack(target);
				}
			}
		}
		// delete all hit enemies
		for (Sprite *target : targetsToDelete) {
			_enemies_2.eraseObject(target);
			this->removeChild(target);
		}
		if (targetsToDelete.size() > 0) {
			projectilesToDelete.pushBack(projectile);
		}
		targetsToDelete.clear();
	}
	// remove projectiles
	for (Sprite *projectile : projectilesToDelete) {
		_projectiles.eraseObject(projectile);
		this->removeChild(projectile);
	}
	projectilesToDelete.clear();
}

void PlaySceneCover::win()
{
	auto scene = GameWinScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void PlaySceneCover::lose()
{
	auto scene = GameLoseScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void PlayScene::lose()
{
	auto scene = GameLoseScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void PlaySceneCover::myclock(float dt)
{ 
	--m_time;
	if (m_time == 0)
	{
		this->lose();
	}
	if (m_time > 0) {
		Label *labelTime = (Label *)this->getChildByTag(11);
		labelTime->setString(StringUtils::format("Time: %d ", m_time));
	}
}

void PlayScene::bullet(float dt){
	judge = 0;
	show_words = false;
}

void PlaySceneCover::myscore(float dt)
{
	if (_numCollected >= 15)
	{
		SimpleAudioEngine::getInstance()->playEffect("win.mp3");
		this->win();
	}
	if (_numCollected<15) {
		Label *score = (Label *)this->getChildByTag(12);
		score->setString(StringUtils::format("SCORE: %d/15", _numCollected));
	}
}

void PlaySceneCover::mystate(float dt)
{
	if (poison == true){
		Label *state = (Label *)this->getChildByTag(13);
		state->setString(StringUtils::format("STATE: %s", "POISONING"));
	}
}

void PlayScene::myusingtime(float dt)
{
	if (get_key == true){
		--using_time;
		if (using_time == 0)
			get_key = false;
	}
}

void PlayScene::myhelper(float dt)
{
	if (get_helper == true){
		int i = 0;
		helper = Sprite::create("Hero_1.png");
		helper->setPosition(special_pos);
		helper->setOpacity(132);
		addChild(helper);
		auto moveBy = MoveBy::create(4.5f, Vec2(-96, 192));
		auto seq = Sequence::create(moveBy, DelayTime::create(0.5), RemoveSelf::create(true), NULL);
		helper->runAction(seq);

		scheduleOnce(schedule_selector(PlayScene::helper_get), 5.0f);

		_numCollected += 2;
		SimpleAudioEngine::getInstance()->playEffect("pickup.wav");

		get_helper = false;
	}
}

void PlayScene::helper_get(float dt){
	Point pos = helper->getPosition();
	Point tileCoord = tileToPosition(pos);
	_meta->removeTileAt(tileCoord);
	_foreground_1->removeTileAt(tileCoord);

	TTFConfig config("fonts/haibaoti.ttf", 30);
	auto _label = Label::createWithTTF(config, StringUtils::format("SCORE+2"));
	_label->setColor(Color3B::RED);
	_label->setPosition(special_pos + Vec2(-32, 192));
	_label->runAction(Sequence::create(FadeIn::create(0.5), DelayTime::create(1.0), FadeOut::create(0.5), RemoveSelf::create(true), NULL));
	addChild(_label);
}

void PlayScene::menuPauseCallback(Object* pSender)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	RenderTexture *renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height);

	renderTexture->begin();
	this->getParent()->visit();
	renderTexture->end();

	Director::getInstance()->pushScene(Gamepause::scene(renderTexture));
}

bool PlaySceneCover::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();

	TTFConfig config("fonts/DFT_YYD.ttf", 30);
	
	labelTime = Label::createWithTTF(config, StringUtils::format("Time: %d ", m_time));
	labelTime->setPosition(Vec2(visibleSize.width / 2 + 300, visibleSize.height - 32));
	labelTime->setTag(11);
	labelTime->setColor(Color3B::BLUE);
	addChild(labelTime);

	score = Label::createWithTTF(config, StringUtils::format("SCORE: %d/15", _numCollected));
	score->setPosition(Vec2(visibleSize.width / 2 + 300, visibleSize.height - 64));
	score->setTag(12);
	score->setColor(Color3B::BLUE);
	addChild(score);

	state = Label::createWithTTF(config, StringUtils::format("STATE: %s", "NORMAL"));
	state->setPosition(Vec2(visibleSize.width / 2 + 300, visibleSize.height - 96));
	state->setTag(13);
	state->setColor(Color3B::BLUE);
	addChild(state);

	schedule(schedule_selector(PlaySceneCover::myclock), 1.0f);
	schedule(schedule_selector(PlaySceneCover::mystate));
	schedule(schedule_selector(PlaySceneCover::myscore));

	MenuItemImage *pCloseItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		this,
		menu_selector(PlayScene::menuPauseCallback));

	pCloseItem->setPosition(Vec2(visibleSize.width - pCloseItem->getContentSize().width / 2,
		visibleSize.height - pCloseItem->getContentSize().height / 2));

	// create menu, it's an autorelease object  
	Menu* pMenu = Menu::create(pCloseItem, NULL);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu, 1);
}