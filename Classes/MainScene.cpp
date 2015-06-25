//
//  MainScene.cpp
//  AntGame
//
//  Created by 長谷川 脩人 on 2015/06/17.
//
//

#include "MainScene.h"
//#include "HelloWorldScene.h"
#include "QueenScene.h"
#include "FoodScene.h"
#include "EggScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
using namespace CocosDenshion;

MainScene::MainScene()
: _second(0)
,_count(0)
,_ants(3)
,_deathCount(0)
,_foods(10)
,_eggs(0)
,_eCount(0)
,_upDown(0)
,_level(1)
,_lvCount(0)
,_aMax(0)
,_fMax(0)
,_eMax(0)
,_secondLabel(NULL)
,_antsLabel(NULL)
,_foodsLabel(NULL)
,_eggsLabel(NULL)
,_lvLabel(NULL)
{
    
}

MainScene::~MainScene()
{
    CC_SAFE_RELEASE_NULL(_secondLabel);
    CC_SAFE_RELEASE_NULL(_antsLabel);
    CC_SAFE_RELEASE_NULL(_foodsLabel);
    CC_SAFE_RELEASE_NULL(_eggsLabel);
    CC_SAFE_RELEASE_NULL(_lvLabel);
}

Scene* MainScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainScene::create();
    scene->addChild(layer);
    return scene;
};

void MainScene::update(float dt)
{
    _count += 1;
    if (_count > 60) {
        
        //秒の増加
        _second += 1;
        int second = static_cast<int>(_second);
        _secondLabel->setString(StringUtils::toString(second));
        
        //時間をユーザーに格納
        UserDefault::getInstance()->setIntegerForKey("secondkey", second);
        
        //食糧の増減
        /*
        if (_second % 3 == 0) {
            _foods += 3 * _ants;
            int foods = static_cast<int>(_foods);
            _foodsLabel->setString(StringUtils::toString(foods));
            UserDefault::getInstance()->setIntegerForKey("foodskey", foods);
        }
         */
        
        if((_second+1) % 3 == 0){
            _foods -= _ants + 1;
            int foods = static_cast<int>(_foods);
            _foodsLabel->setString(StringUtils::toString(foods));
            UserDefault::getInstance()->setIntegerForKey("foodskey", foods);
        }
        
        //卵の孵化
        if(_eggs == 0){
            _eCount = 0;
        }else{
            _eCount += 1;
        }
        if (_eggs > 0) {//卵が一つでもある場合
            if(_ants <= _aMax){//アリが最大値ではない場合
                if (_eCount > 10) {//１０秒で一匹卵が孵化する
                    int eggs = static_cast<int>(_eggs);
                    eggs -= 1;
                    _eggs = eggs;
                    _eggsLabel->setString(StringUtils::toString(eggs));
                    UserDefault::getInstance()->setIntegerForKey("eggskey", eggs);
                    
                    int ants = static_cast<int>(_ants);
                    ants += 1;
                    _ants = ants;
                    _antsLabel->setString(StringUtils::toString(ants));
                    UserDefault::getInstance()->setIntegerForKey("antskey", ants);
                    
                    _eCount = 0;
                }
            }
        }
        
        //アリの死
        if (_ants > 0) {//アリが一匹でもいる場合
            _deathCount += 1;
            if (_deathCount > 15) {//15秒で一匹アリが減る
                int ants = static_cast<int>(_ants);
                ants -= 1;
                _ants = ants;
                _antsLabel->setString(StringUtils::toString(ants));
                UserDefault::getInstance()->setIntegerForKey("antskey", ants);
                _deathCount = 0;
            }
        }
        
        //レベルアップ
        if(_ants == _aMax){
            _lvCount = 1;
        }
        if (_lvCount == 1) {
            _level += 1;
            int level = static_cast<int>(_level);
            _lvLabel->setString(StringUtils::toString(level));
            UserDefault::getInstance()->setIntegerForKey("levelkey", level);
            
            //レベルから最大値を取得
            _aMax = _level*10;
            _fMax = _level*800;
            _eMax = _level*10;
            
            _lvCount = 0;
        }
        
        //最大値の場合ラベルを赤く表示
        if (_foods < _fMax) {
            _foodsLabel->setColor(Color3B(255, 255, 255));
        }else{
            _foodsLabel->setColor(Color3B(255, 0, 0));
        }
        if (_eggs < _eMax) {
            _eggsLabel->setColor(Color3B(255, 255, 255));
        }else{
            _eggsLabel->setColor(Color3B(255, 0, 0));
        }

        _count = 0;
    }
}


bool MainScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("BGM_UnderGround.m4a");
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0);
    SimpleAudioEngine::getInstance()->playBackgroundMusic("BGM_UnderGround.m4a",true);
    
    auto director = Director::getInstance();//ディレクター
    auto size = director->getWinSize();
    
    //ユーザーから情報を取得
    ///*
    auto userDefault = UserDefault::getInstance();
    _second = userDefault->getIntegerForKey("secondkey");
    _foods = userDefault->getIntegerForKey("foodskey");
    _eggs = userDefault->getIntegerForKey("eggskey");
    _ants = userDefault->getIntegerForKey("antskey");
    _level = userDefault->getIntegerForKey("levelkey");
    //*/
    /*
    UserDefault::getInstance()->setIntegerForKey("secondkey", 0);
    UserDefault::getInstance()->setIntegerForKey("foodskey", 30);
    UserDefault::getInstance()->setIntegerForKey("eggskey", 0);
    UserDefault::getInstance()->setIntegerForKey("antskey", 3);
    UserDefault::getInstance()->setIntegerForKey("levelkey", 1);
    */
    
    //レベルから最大値を取得
    _aMax = _level*10;
    _fMax = _level*800;
    _eMax = _level*10;
    
    
    //背景
    auto background = Sprite::create("img/bg.png");
    background->setPosition(Vec2(size.width/2,-480));
    this->addChild(background);
     
    
    //ラベル
    //秒数
    this->scheduleUpdate();
    int second = static_cast<int>(_second);
    auto secondLabel = Label::createWithSystemFont(StringUtils::toString(second), "Arial", 16);
    this->setSecondLabel(secondLabel);
    secondLabel->setPosition(Vec2(size.width/2 - 200, size.height - 30));
    this->addChild(secondLabel);
    //食糧
    int foods = static_cast<int>(_foods);
    auto foodsLabel = Label::createWithSystemFont(StringUtils::toString(foods), "Arial", 16);
    this->setFoodsLabel(foodsLabel);
    foodsLabel->setPosition(Vec2(size.width/2 - 160, size.height - 30));
    this->addChild(foodsLabel);
    //卵
    int eggs = static_cast<int>(_eggs);
    auto eggsLabel = Label::createWithSystemFont(StringUtils::toString(eggs), "Arial", 16);
    this->setEggsLabel(eggsLabel);
    eggsLabel->setPosition(Vec2(size.width/2 - 120, size.height - 30));
    this->addChild(eggsLabel);
    //蟻
    int ants = static_cast<int>(_ants);
    auto antLabel = Label::createWithSystemFont(StringUtils::toString(ants), "Arial", 16);
    this->setAntsLabel(antLabel);
    antLabel->setPosition(Vec2(size.width/2 - 80, size.height - 30));
    this->addChild(antLabel);
    //レベル
    int level = static_cast<int>(_level);
    auto lvLabel = Label::createWithSystemFont(StringUtils::toString(level), "Arial", 16);
    this->setLvLabel(lvLabel);
    lvLabel->setPosition(Vec2(size.width/2 - 10, size.height - 30));
    this->addChild(lvLabel);
    
    
    //タッチ
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [](Touch* touch, Event* event){
        return true;
    };
    listener->onTouchMoved = [](Touch* touch, Event* event){
        
    };
    director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    //ボタン
    auto Qbutton = MenuItemImage::create("img/Qroom.png","img/Qroom.png",[](Ref* ref){
        //シーン移動
        Director::getInstance()->replaceScene(TransitionFade::create( 1.0f,QueenScene::createScene()));
    });
    auto Fbutton = MenuItemImage::create("img/Froom.png","img/Froom.png",[](Ref* ref){
        //シーン移動
        Director::getInstance()->replaceScene(TransitionFade::create( 1.0f,FoodScene::createScene()));
    });
    auto Ebutton = MenuItemImage::create("img/Eroom.png","img/Eroom.png",[](Ref* ref){
        //シーン移動
        Director::getInstance()->replaceScene(TransitionFade::create( 1.0f,EggScene::createScene()));
    });
    auto menu = Menu::create(Qbutton,Fbutton,Ebutton,NULL);
    menu->setPosition(Vec2(size.width-100, 50));
    menu->alignItemsHorizontally();
    this->addChild(menu);
    
    //画面上下ボタン
    if(_upDown != 5){//一番下では下ボタンを非表示
        auto downButton = MenuItemImage::create("img/down.png","img/down.png",[&](Ref* ref){
            
        });
        auto dMenu = Menu::create(downButton,NULL);
        dMenu->setPosition(Vec2(size.width/2,5));
        this->addChild(dMenu);
        _upDown += 1;
        log("%i",_upDown);
    }
    if (_upDown != 0) {//一番上では上ボタンを非表示
        auto upButton = MenuItemImage::create("img/up.png","img/up.png",[&](Ref* ref){
            
        });
        auto uMenu = Menu::create(upButton,NULL);
        uMenu->setPosition(Vec2(size.width/2,size.height - 5));
        this->addChild(uMenu);
        _upDown -= 1;
        log("%i",_upDown);
    }
    
    return true;
}