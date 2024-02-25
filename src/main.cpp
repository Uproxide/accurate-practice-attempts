#include <Geode/Geode.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
using namespace geode::prelude;

int totalAttempts;
int practiceAttempts;
bool practiceMode = false;
bool platformer;

class $modify(PlayLayer) {
	bool init(GJGameLevel* p0, bool p1, bool p2) {
		if (!PlayLayer::init(p0, p1, p2)) {
			return false;
		}

		totalAttempts = 1;
		practiceAttempts = 0;
		practiceMode = false;

		if (p0->isPlatformer()) {
			platformer = true;
		} 
		else {
			platformer = false;
		}

		return true;
	}

	void updateAttempts() {
		PlayLayer::updateAttempts();

		totalAttempts++;

		if (m_isPracticeMode) {
			practiceAttempts++;
		}

		log::info("{}", totalAttempts);
	}
};

class $modify(PauseLayer) {
	void onPracticeMode(CCObject* sender) {
		PauseLayer::onPracticeMode(nullptr);

		practiceAttempts = 1;
		practiceMode = true;
	}
};

class $modify(EndLevelLayer) {
	void customSetup() {
		EndLevelLayer::customSetup();
		
		auto attemptsLabel = static_cast<cocos2d::CCLabelBMFont*>(this->getChildByIDRecursive("attempts-label"));
		auto jumpsLabel = static_cast<cocos2d::CCLabelBMFont*>(this->getChildByIDRecursive("jumps-label"));
		auto timeLabel = static_cast<cocos2d::CCLabelBMFont*>(this->getChildByIDRecursive("time-label"));
		
		if (practiceMode && !platformer) {
			auto practiceAttemptsLabel = CCLabelBMFont::create("Practice Attempts: 1", "goldFont.fnt");

			timeLabel->setPositionY(timeLabel->getPositionY() - 20);
			jumpsLabel->setPositionY(jumpsLabel->getPositionY() - 20);
			attemptsLabel->setPositionY(attemptsLabel->getPositionY() + 5);

			practiceAttemptsLabel->setPositionX(attemptsLabel->getPositionX());
			practiceAttemptsLabel->setPositionY(attemptsLabel->getPositionY() - 20);
			practiceAttemptsLabel->setScale(0.8f);
			practiceAttemptsLabel->setID("practice-attempts-label"_spr);
			std::string pracAtt = fmt::format("Practice Attempts: {}", std::to_string(practiceAttempts));
			practiceAttemptsLabel->setString(pracAtt.c_str());


			this->addChild(practiceAttemptsLabel);
		}
		else if (!practiceMode) {
			if (this->getChildByID("uproxide.accupracatt/practice-attempts-label")) {
				this->removeChildByID("uproxide.accupracatt/practice-attempts-label");
				timeLabel->setPositionY(timeLabel->getPositionY() + 20);
				jumpsLabel->setPositionY(jumpsLabel->getPositionY() + 20);
				attemptsLabel->setPositionY(attemptsLabel->getPositionY() - 5);
			}
		}
	}
};
