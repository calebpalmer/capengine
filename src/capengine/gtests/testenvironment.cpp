#include "testenvironment.h"
#include "capengine/game_management.h"

namespace CapEngine::testing {

	TestEnvironment* TestEnvironment::gTestEnvironment = nullptr;

	//constructor
	TestEnvironment::TestEnvironment(){
		m_windowId = CapEngine::init(CapEngine::WindowParams{"Tests", 0, 0, 0, false, false, false, false, "Tests"});
	}

	//descructor
	TestEnvironment::~TestEnvironment(){
		CapEngine::end();
	}


	 TestEnvironment* TestEnvironment::instance(){
		 if(TestEnvironment::gTestEnvironment == nullptr){
			 TestEnvironment::gTestEnvironment = new TestEnvironment();
		}
		 return TestEnvironment::gTestEnvironment;
	}
}
