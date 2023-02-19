#ifndef CAPENGINE_EVENTSUBSCRIBERTESTS_H
#define CAPENGINE_EVENTSUBSCRIBERTESTS_H

#include <cppunit/TestAssert.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>
#include <optional>

#include "../EventDispatcher.h"
#include "../eventsubscriber.h"
#include "../gameevent.h"

namespace CapEngine
{
namespace Testing
{

class TestGameEvent : public GameEvent
{
  public:
    ~TestGameEvent() override = default;
    std::string type() const override { return "TestGameEvent"; }
};

class EventSubscriberTest : public CppUnit::TestFixture
{
  public:
    void testSubscribeGameEvent();

    CPPUNIT_TEST_SUITE(EventSubscriberTest);
    CPPUNIT_TEST(testSubscribeGameEvent);
    CPPUNIT_TEST_SUITE_END();
};

void EventSubscriberTest::testSubscribeGameEvent()
{
    EventSubscriber eventSubscriber(std::nullopt);

    bool called = false;

    auto connection = eventSubscriber.m_gameEventSignal.connect(
        [&called](const GameEvent &event) {
            CPPUNIT_ASSERT("TestGameEvent" == event.type());
            called = true;
        });

    TestGameEvent testGameEvent;
    eventSubscriber.m_gameEventSignal(testGameEvent);
    CPPUNIT_ASSERT(true == called);
}

} // namespace Testing
} // namespace CapEngine

#endif
