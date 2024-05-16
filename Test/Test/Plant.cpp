#include "Plant.hpp"
#include <iostream>

bool Plant::Hydrate(Irrigation irrigationType, float waterAmount) {
	if (HydrationPercentage + waterAmount <= 100) HydrationPercentage + waterAmount;
	else return false;
	return true;
}

Growth Plant::IncreaseGrowthStage() {
	 if (Growthstage == Withering)std::cout << "Plant has died";
	 else Growthstage + 1;
	 return Growthstage;
}
Growth Plant::DecreaseGrowthStage() {
	if (Growthstage == Sprout)std::cout << "Plant has been reseeded and is sprouting";
	else Growthstage - 1;
	return Growthstage;
}