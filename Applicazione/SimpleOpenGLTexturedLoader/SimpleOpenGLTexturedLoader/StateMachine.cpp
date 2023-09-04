#include "StateMachine.h"

extern Mouse mouse;
extern StateMachine stateMachine;
extern Ship ship;
extern Render render;
extern Sounds sounds;
extern Enemy enemy;
extern Score score;

StateMachine::StateMachine(void) {
	SetState(MENU);
	SetOldState(MENU);
}

StateMachine::~StateMachine(void) {}


void StateMachine::SetState(int state) {
	current_state = state;
}

int StateMachine::GetCurrentState(void) {
	return current_state;
}

void StateMachine::SetOldState(int state) {
	old_state = state;
}

int StateMachine::GetOldState(void) {
	return old_state;
}

GLint StateMachine::GenerateRandomMapIndex() {
	long int t = static_cast<long int>(time(0));
	GLint rand = t % 3; //it gets a value between [0, 2]
	return ++rand; //[1, 3]
}


void StateMachine::ChangeState(void) { //IDLE Function
	int _state = stateMachine.GetCurrentState();

	/* Delta Time */
	t = glutGet(GLUT_ELAPSED_TIME);
	dt = (t - old_t) / 1000.0;
	old_t = t;
	/************************************************************/

	switch (_state) {
	case MENU:
		if (stateMachine.changed) {
			stateMachine.changed = false;
			stateMachine.soundtrackStart = true;
			ship.InitShip(); //normal speed
		}
		/* Music */
		sounds.PlayWaves(false);
		sounds.PlayVictory(false);
		sounds.PlayGameOver(false);

		if (stateMachine.soundtrackStart) {
			stateMachine.soundtrackStart = false;
			sounds.PlayMenuSoundtrack(true);
		}
		///////////////////////////////////////
		if (mouse.StartPressed()) {
			cout << "START ---------------------------\n";
			sounds.PlayMenuSoundtrack(false);
			sounds.PlayClick();
			stateMachine.soundtrackStart = true;

			//Decide Random Map
			randomMap = GenerateRandomMapIndex();
			cout << "RANDOM = " << randomMap << endl;

			stateMachine.changed = true;
			stateMachine.SetOldState(MENU);
			stateMachine.SetState(LEVEL_1);
			render.first = true;
		}
		if (mouse.RulesPressed()) {
			cout << "RULES ---------------------------\n";
			sounds.PlayClick();
			stateMachine.changed = true;
			stateMachine.SetOldState(MENU);
			stateMachine.SetState(RULES);
			render.first = true;
		}
		if (mouse.CommandPressed()) {
			cout << "COMMAND ---------------------------\n";
			sounds.PlayClick();
			stateMachine.changed = true;
			stateMachine.SetOldState(MENU);
			stateMachine.SetState(COMMANDS);
			render.first = true;
		}
		if (mouse.StartHover() || mouse.RulesHover() || mouse.CommandHover()) {
			stateMachine.changed = true;
			hover_flag = true;
		}
		else if (hover_flag) {
			stateMachine.changed = true;
			hover_flag = false;
		}
		break;
	case RULES:
		if (mouse.BackToMenuRulesPressed()) {
			cout << "BACK TO MENU FROM RULES ---------------------------\n";
			sounds.PlayClick();
			//sounds.PlayVictorySoundtrack(false);
			//stateMachine.soundtrackStart = true;

			stateMachine.changed = true;
			stateMachine.SetOldState(stateMachine.GetCurrentState());
			stateMachine.SetState(MENU);
			render.first = true;
		}
		if (mouse.BackToMenuRulesHover()) {
			stateMachine.changed = true;
			hover_flag = true;
		}
		else if (hover_flag) {
			stateMachine.changed = true;
			hover_flag = false;
		}
		break;
	case COMMANDS:
		if (mouse.BackToMenuCommandsPressed()) {
			cout << "BACK TO MENU FROM COMMANDS ---------------------------\n";
			sounds.PlayClick();
			//sounds.PlayVictorySoundtrack(false);
			//stateMachine.soundtrackStart = true;

			stateMachine.changed = true;
			stateMachine.SetOldState(stateMachine.GetCurrentState());
			stateMachine.SetState(MENU);
			render.first = true;
		}
		if (mouse.BackToMenuCommandsHover()) {
			stateMachine.changed = true;
			hover_flag = true;
		}
		else if (hover_flag) {
			stateMachine.changed = true;
			hover_flag = false;
		}
		break;
	case LEVEL_1:
		if (stateMachine.changed) {
			stateMachine.changed = false;
			sounds.PlayWaves(false);
			sounds.PlayWaves(true);
			ship.InitShip(); //normal speed
		}
		ship.IdleForwardMovement(); //Idle ship movement starts
		break;
	case LEVEL_2:
		if (stateMachine.changed) {
			stateMachine.changed = false;
			sounds.PlayWaves(false);
			sounds.PlayWaves(true);
			ship.InitShip(); //medium speed;
		}
		ship.IdleForwardMovement(); //Idle ship movement starts
		break;
	case LEVEL_3:
		if (stateMachine.changed) {
			stateMachine.changed = false;
			sounds.PlayWaves(false);
			sounds.PlayWaves(true);
			ship.InitShip(); //higher speed
		}
		ship.IdleForwardMovement(); //Idle ship movement starts
		break;
	case GAMEOVER:
		sounds.PlayEnemyAppearing(false);
		if (mouse.TryAgainPressed()) {
			cout << "TRY AGAIN ---------------------------\n";
			sounds.PlayClick();
			
			//Decide Random Map
			randomMap = GenerateRandomMapIndex();
			cout << "RANDOM = " << randomMap << endl;

			stateMachine.changed = true;
			stateMachine.SetState(stateMachine.GetOldState());
			render.first = true;
			ship.InitShip();
			enemy.InitEnemy();
			score.InitScore();
			sounds.PlayVictory(false);
			sounds.PlayGameOver(false);
		}
		if (mouse.BackToMenuGameoverPressed()) {
			cout << "BACK TO MENU FROM GAMEOVER ---------------------------\n";
			sounds.PlayClick();
			//sounds.PlayVictorySoundtrack(false);
			//stateMachine.soundtrackStart = true;

			stateMachine.changed = true;
			stateMachine.SetOldState(stateMachine.GetCurrentState());
			stateMachine.SetState(MENU);
			ship.InitShip();
			enemy.InitEnemy();
			score.InitScore();
		}
		if (mouse.TryAgainHover() || mouse.BackToMenuGameoverHover()) {
			stateMachine.changed = true;
			hover_flag = true;
		}
		else if (hover_flag) {
			stateMachine.changed = true;
			hover_flag = false;
		}
		stateMachine.changed = true;
		sounds.PlayWaves(false);
		break;
	case VICTORY_1:
	case VICTORY_2:
	case VICTORY_3:
		sounds.PlayEnemyAppearing(false);

		if (mouse.NextLevelPressed()) {
			cout << "NEXT LEVEL ---------------------------\n";
			sounds.PlayClick();
			
			//Decide Random Map
			randomMap = GenerateRandomMapIndex();
			cout << "RANDOM = " << randomMap << endl;

			stateMachine.changed = true;
			score.SaveScore(stateMachine.GetOldState());
			stateMachine.SetOldState(stateMachine.GetCurrentState());
			stateMachine.SetState(stateMachine.GetCurrentState() + 1);
			render.first = true;

			sounds.PlayVictory(false);
			sounds.PlayGameOver(false);
		}
		if (mouse.BackToMenuVictoryPressed()) {
			cout << "BACK TO MENU FROM VICTORY ---------------------------\n";
			sounds.PlayClick();

			stateMachine.changed = true;
			score.SaveScore(stateMachine.GetOldState());
			stateMachine.SetOldState(stateMachine.GetCurrentState());
			stateMachine.SetState(MENU);
			render.first = true;
		}
		if (mouse.BackToMenuVictoryHover() || mouse.NextLevelHover()) {
			stateMachine.changed = true;
			hover_flag = true;
		}
		else if (hover_flag) {
			stateMachine.changed = true;
			hover_flag = false;
		}
		break;
	default:
		stateMachine.changed = false;
		stateMachine.changedWindow = false;
		break;
	}

	if (stateMachine.changed) {
		//stateMachine.changedWindow = false;
		glutPostRedisplay();
	}

	/*
	if ((GetCurrentState() == VICTORY_1 || GetCurrentState() == VICTORY_2 || GetCurrentState() == VICTORY_1 || GetCurrentState() == GAMEOVER ||
		GetCurrentState() == LEVEL_1 || GetCurrentState() == LEVEL_2 || GetCurrentState() == LEVEL_3) && stateMachine.changedWindow) {

		stateMachine.changedWindow = false;
		glutPostRedisplay();
	}
	*/

	if (GetCurrentState() == LEVEL_1 || GetCurrentState() == LEVEL_2 || GetCurrentState() == LEVEL_3) {
		glutPostRedisplay();
	}

}