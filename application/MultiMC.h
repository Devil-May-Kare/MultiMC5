#pragma once

#include <QApplication>
#include <memory>
#include <QDebug>
#include <QFlag>
#include <QIcon>
#include <QDateTime>

class QFile;
class MetaPackageList;
class HttpMetaCache;
class SettingsObject;
class InstanceList;
class IconList;
class QNetworkAccessManager;
class ForgeVersionList;
class LiteLoaderVersionList;
class JavaVersionList;
class UpdateChecker;
class TranslationDownloader;
class AccountModel;

#if defined(MMC)
#undef MMC
#endif
#define MMC (static_cast<MultiMC *>(QCoreApplication::instance()))

enum UpdateFlag
{
	None = 0x0,
	RestartOnFinish = 0x1,
	DryRun = 0x2,
	OnExit = 0x4
};
Q_DECLARE_FLAGS(UpdateFlags, UpdateFlag);
Q_DECLARE_OPERATORS_FOR_FLAGS(UpdateFlags);

class MultiMC : public QApplication
{
	// friends for the purpose of limiting access to deprecated stuff
	friend class MultiMCPage;
	friend class MainWindow;
	Q_OBJECT
public:
	enum Status
	{
		Failed,
		Succeeded,
		Initialized
	};

public:
	MultiMC(int &argc, char **argv, bool test_mode = false);
	virtual ~MultiMC();

	// InstanceList, IconList, OneSixFTBInstance, LegacyUpdate, LegacyInstance, MCEditTool, JVisualVM, MinecraftInstance, JProfiler, BaseInstance
	std::shared_ptr<SettingsObject> settings()
	{
		return m_settings;
	}

	qint64 timeSinceStart() const
	{
		return startTime.msecsTo(QDateTime::currentDateTime());
	}

	QIcon getThemedIcon(const QString& name);

	void setIconTheme(const QString& name);

	// DownloadUpdateTask
	std::shared_ptr<UpdateChecker> updateChecker()
	{
		return m_updateChecker;
	}

	std::shared_ptr<JavaVersionList> javalist();

	// APPLICATION ONLY
	std::shared_ptr<InstanceList> instances()
	{
		return m_instances;
	}

	std::shared_ptr<AccountModel> accountsModel()
	{
		return m_accountsModel;
	}

	// APPLICATION ONLY
	Status status()
	{
		return m_status;
	}

	// APPLICATION ONLY
	void installUpdates(const QString updateFilesDir, UpdateFlags flags = None);

protected: /* to be removed! */
	// FIXME: remove. used by MultiMCPage to enumerate translations.
	/// this is the static data. it stores things that don't move.
	const QString &staticData()
	{
		return staticDataPath;
	}

	// FIXME: remove. used by MainWindow to create application update tasks
	/// this is the root of the 'installation'. Used for automatic updates
	const QString &root()
	{
		return rootPath;
	}

private slots:
	/**
	 * Do all the things that should be done before we exit
	 */
	void onExit();

private:
	void initVersionLists();
	void initLogger();
	void initIcons();
	void initGlobalSettings(bool test_mode);
	void initTranslations();

private:
	friend class UpdateCheckerTest;
	friend class DownloadTaskTest;

	QDateTime startTime;

	std::shared_ptr<QTranslator> m_qt_translator;
	std::shared_ptr<QTranslator> m_mmc_translator;
	std::shared_ptr<SettingsObject> m_settings;
	std::shared_ptr<InstanceList> m_instances;
	std::shared_ptr<UpdateChecker> m_updateChecker;
	std::shared_ptr<AccountModel> m_accountsModel;
	std::shared_ptr<LiteLoaderVersionList> m_liteloaderlist;
	std::shared_ptr<JavaVersionList> m_javalist;
	std::shared_ptr<TranslationDownloader> m_translationChecker;

	QString m_updateOnExitPath;
	UpdateFlags m_updateOnExitFlags = None;

	QString rootPath;
	QString staticDataPath;
	QString dataPath;

	Status m_status = MultiMC::Failed;
public:
	std::shared_ptr<QFile> logFile;
};
