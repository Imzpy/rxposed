另一种方法和思路，进行ActivityThread 代理

6137          mBoundApplication = data;

mBoundApplication 的结构体是  AppBindData

755      static final class AppBindData {
756          @UnsupportedAppUsage
757          LoadedApk info;
758          @UnsupportedAppUsage
759          String processName;
760          @UnsupportedAppUsage
761          ApplicationInfo appInfo;
762          @UnsupportedAppUsage
763          List<ProviderInfo> providers;
764          ComponentName instrumentationName;
765          @UnsupportedAppUsage
766          Bundle instrumentationArgs;
767          IInstrumentationWatcher instrumentationWatcher;
768          IUiAutomationConnection instrumentationUiAutomationConnection;
769          int debugMode;
770          boolean enableBinderTracking;
771          boolean trackAllocation;
772          @UnsupportedAppUsage
773          boolean restrictedBackupMode;
775          boolean persistent;
776          Configuration config;
777          @UnsupportedAppUsage(maxTargetSdk = Build.VERSION_CODES.P, trackingBug = 115609023)
778          CompatibilityInfo compatInfo;
779          String buildSerial;


后续更