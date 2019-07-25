float distanceFromLine = 0;

void FirstGoal(){
	RunToLine();
	Run();
	Turn(-1);
	RunExactly(200);
	RunExactly(-250);
	TurnExactly(70);
	RunExactly(30);
	Run();
	Run();
	return 0;
}

bool GetBall(){
	if (ScanBall())
		{
			KeepBall();
			RunExactlyOnLine(50);
			RunExactly(-50);
			return true;
		}
	else{
			RunExactlyOnLine(150);
		}
		return false;
}

void ReturnToLine(){
	RunExactly(-150);
	TurnExactly(180);
	RunExactly(-200);
	Run();
	return;
}

void RunToGoal(){
	Run();
	RunExactly(400);
	TurnExactly(-180);
	return;
}

void RunFromGoal(){
	Run();
	Run();
	return;
}

void Goal(){
	for (int i = 1; i <= 6; i++)
	{
		TurnTable(i);
		delay(500);
		PushBall();
		delay(500);
	}
	return;
}