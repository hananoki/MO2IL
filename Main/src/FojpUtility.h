#pragma once

//////////////////////////////////////////////////////////////////////////////////
class FojpUtility : public QObject {
	Q_OBJECT
public:
	FojpUtility();
	~FojpUtility();

	void convert();

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

