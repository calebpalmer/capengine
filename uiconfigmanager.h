#ifndef CAPENGINE_UICONFIGMANAGER_H
#define CAPENGINE_UUICONFIGMANAGER_H

#include <boost/optional.hpp>

namespace CapEngine { namespace UI {

		const std::string kDefaultFontSettingsPath = "/font/path";

		class UIConfigManager {
		public:
			static UIConfigManager& getInstance();

			boost::optional<std::string> getSetting(const std::string &settingPath);

		private:
			UIConfigManager();
		};

	}}

#endif // CAPENGINE_UICONFIGMANAGERH_
