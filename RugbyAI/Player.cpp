#include "Player.h"
#include "RugbyScene.h"

#include "StateMachine.h"
#include "RugbyPlayerAction.h"
#include "RugbyPlayerCondition.h"

#include "Debug.h"

void Player::OnInitialize()
{
	mpStateMachine = new StateMachine<Player>(this, (int)State::Count);
	mInvincibilityDuration = 1.0f;
	mSpeedBoostDuration = 1.5f;
	mIsInvincible = false;

	// Start
	Action<Player>* action = mpStateMachine->CreateAction<RugbyPlayerAction_Start>(State::Start);
	Transition<Player>* transition = action->CreateTransition(State::Try);
	transition->AddCondition<RugbyPlayerCondition_HasBall>();
	transition = action->CreateTransition(State::Support);
	transition->AddCondition<RugbyPlayerCondition_AllyHasBall>();
	transition = action->CreateTransition(State::Defense);
	transition->AddCondition<RugbyPlayerCondition_AllyHasBall>(false);

	// Try
	action = mpStateMachine->CreateAction<RugbyPlayerAction_Try>(State::Try);
	transition = action->CreateTransition(State::Defense);
	transition->AddCondition<RugbyPlayerCondition_AllyHasBall>(false);
	transition = action->CreateTransition(State::Pass);
	transition->AddCondition<RugbyPlayerCondition_HasBall>();
	transition->AddCondition<RubgyPlayerCondition_ShouldPass>();
	transition->AddCondition<RugbyPlayerCondition_HasPlayerAbleToReceivePass>();

	// Support
	action = mpStateMachine->CreateAction<RugbyPlayerAction_Support>(State::Support);
	transition = action->CreateTransition(State::Try);
	transition->AddCondition<RugbyPlayerCondition_HasBall>();
	transition = action->CreateTransition(State::Defense);
	transition->AddCondition<RugbyPlayerCondition_AllyHasBall>(false);
	transition->AddCondition<RugbyPlayerCondition_HasBall>(false);

	// Defense
	action = mpStateMachine->CreateAction<RubgyPlayerAction_Defense>(State::Defense);
	transition = action->CreateTransition(State::Try);
	transition->AddCondition<RugbyPlayerCondition_HasBall>();
	transition = action->CreateTransition(State::Support);
	transition->AddCondition<RugbyPlayerCondition_AllyHasBall>();

	// Pass
	action = mpStateMachine->CreateAction<RubgyPlayerAction_Pass>(State::Pass);
	transition = action->CreateTransition(State::Defense);
	transition->AddCondition<RugbyPlayerCondition_AllyHasBall>(false);
	transition = action->CreateTransition(State::Support);
	transition->AddCondition<RugbyPlayerCondition_AllyHasBall>();

	mpStateMachine->SetState(State::Start);
}

void Player::OnUpdate()
{
	if (mDisplacementBoundingBox == nullptr) return;

	const sf::Vector2f topLeftPosition = GetPosition(0.f, 0.f);
	const sf::Vector2f botRightPosition = GetPosition(1.f, 1.f);

	int newPosX = topLeftPosition.x;
	int newPosY = topLeftPosition.y;
	if (topLeftPosition.x < mDisplacementBoundingBox->xMin)
	{
		newPosX = mDisplacementBoundingBox->xMin;
	}
	if (botRightPosition.x > mDisplacementBoundingBox->xMax)
	{
		newPosX = mDisplacementBoundingBox->xMax - (botRightPosition.x - topLeftPosition.x);
	}
	if (topLeftPosition.y < mDisplacementBoundingBox->yMin)
	{
		newPosY = mDisplacementBoundingBox->yMin;
	}
	if (botRightPosition.y > mDisplacementBoundingBox->yMax)
	{
		newPosY = mDisplacementBoundingBox->yMax - (botRightPosition.y - topLeftPosition.y);
	}
	SetPosition(newPosX, newPosY, 0.f, 0.f);

	// Draw state
	const sf::Vector2f position = GetPosition();
	const char* stateName = GetStateName((Player::State)mpStateMachine->GetCurrentState());
	Debug::DrawText(position.x, position.y - 50, stateName, 0.5f, 0.5f, sf::Color::Black, 1.f, sf::Color::White);

	// Draw name
	Debug::DrawText(position.x, position.y, GetName(), 0.5f, 0.5f, sf::Color::Black);

	// Draw invincibility
	if (IsInvincible())
	{
		Debug::DrawOutlinedCircle(position.x, position.y, GetRadius() - 4.f, 4.f, sf::Color::Black);
	}

	mpStateMachine->Update();
}

void Player::OnCollision(Entity* collidedWith)
{
	if (IsInvincible() || !HasBall() || collidedWith->IsTag(GetTag()) || collidedWith->IsTag(RugbyScene::Tag::RugbyBall)) return;

	RugbyScene* scene = GetScene<RugbyScene>();
	if (Player* player = dynamic_cast<Player*>(collidedWith))
	{
		scene->GiveBallToPlayer(player);
	}
}

void Player::OnDestroy()
{
}

bool Player::HasBall() const
{
	if (RugbyScene* scene = GetScene<RugbyScene>())
	{
		return scene->GetBall()->GetOwner() == this;
	}
	return false;
}

void Player::PassBall(Player* target)
{
	if (target == nullptr) return;

	if (RugbyScene* scene = GetScene<RugbyScene>())
	{
		scene->GetBall()->SetTarget(target);
	}
}

void Player::SetIsInvicible(bool isInvincible)
{
	mIsInvincible = isInvincible;
}

const char* Player::GetStateName(State state)
{
	switch (state)
	{
	case State::Start: return "Start";
	case State::Try: return "Try";
	case State::Support: return "Support";
	case State::Pass: return "Pass";
	case State::Defense: return "Defense";
	default: return "Unknown";
	}
}
