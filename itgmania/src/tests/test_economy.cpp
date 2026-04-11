#include "global.h"
#include "test_misc.h"
#include "Economy/EconomyManager.h"
#include "RageLog.h"

void TestTransfer()
{
	LOG->Info("Test: Transfer");

	EconomyManager* eco = EconomyManager::Instance();
	eco->Initialize();

	long long startBal = eco->GetBalance("WALLET_PLAYER");
	if( startBal != 100 )
		LOG->Warn("FAIL: Initial Balance is %lld, expected 100", startBal);
	else
		LOG->Info("PASS: Initial Balance");

	eco->RegisterUser("TEST_USER");
	long long testBal = eco->GetBalance("WALLET_TEST_USER");
	if( testBal != 50 )
		LOG->Warn("FAIL: New User Balance is %lld, expected 50", testBal);
	else
		LOG->Info("PASS: New User Balance");

	bool res = eco->Transfer("WALLET_PLAYER", "WALLET_TEST_USER", 10, "Test Tx");
	if( res && eco->GetBalance("WALLET_PLAYER") == 90 && eco->GetBalance("WALLET_TEST_USER") == 60 )
		LOG->Info("PASS: Transfer Logic");
	else
		LOG->Warn("FAIL: Transfer Logic");
}

void TestElo()
{
	LOG->Info("Test: Elo");
	EconomyManager* eco = EconomyManager::Instance();

	int startElo = eco->GetPlayerElo(); // 1200
	eco->UpdateElo(true, 1200); // Win vs equal

	int endElo = eco->GetPlayerElo();
	if( endElo > startElo )
		LOG->Info("PASS: Elo Increase (%d -> %d)", startElo, endElo);
	else
		LOG->Warn("FAIL: Elo did not increase");
}

int main( int argc, char *argv[] )
{
	test_handle_args( argc, argv );
	test_init();

	TestTransfer();
	TestElo();

	test_deinit();
	return 0;
}
