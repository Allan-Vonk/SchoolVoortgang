#pragma once

enum Growth {
	Sprout,
	Seedling,
	Vegetative,
	Budding,
	Flowering,
	Ripening,
	Withering
};
enum Irrigation {
	Localized,
	Drip,
	Sprinkler,
	Basin
};

class IGrowable {
public:
	virtual Growth IncreaseGrowthStage();
	virtual Growth DecreaseGrowthStage();
};

class Plant : IGrowable {
public:
	Growth Growthstage;
	int HydrationPercentage;
	int NitrateValue;
	bool Hydrate(Irrigation, float);
	virtual Growth IncreaseGrowthStage();
	virtual Growth DecreaseGrowthStage();

};