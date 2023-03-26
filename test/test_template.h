#ifndef TEST_TEMPLATE_H
#define TEST_TEMPLATE_H

#include "gmock/gmock.h"

class TemplateTest : public testing::Test {
public:
	TemplateTest() = default;

protected:
	virtual void SetUp();
	virtual void TearDown();
};

#endif  // TEST_TEMPLATE_H
