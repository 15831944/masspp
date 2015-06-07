#define Version    "2.7.5"

[Setup]
AppName=Mass++
AppVerName=Mass++ ver. {#Version} (32-bit)
DefaultDirName={code:GetDefaultDir}
AppendDefaultDirName=No
VersionInfoVersion={#Version}.0
VersionInfoDescription=Mass++ ver. {#Version} (32-bit) Setup
AppCopyright=Mass++ Users Group

OutputBaseFilename=MassppSetup-x86
SourceDir=..\..
OutputDir=build\InnoSetup\output
DefaultGroupName=Mass++
ChangesAssociations=Yes
LicenseFile=doc\release-doc\License-BSD.txt
ArchitecturesAllowed=x86 x64

[Types]
Name: "full"; Description: "Full installation"
Name: "custom"; Description: "Custom installation"; Flags: iscustom

[Components]
Name: "programfiles"; Description: "Program Files"; Types: full custom; Flags: fixed

[Files]
Source: "build\msvs\stage\Win32-Release\bin\*"; DestDir: "{app}\bin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\conf\*"; DestDir: "{app}\conf"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\opt\jre7\*"; DestDir: "{app}\opt\jre7"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
;Source: "build\msvs\stage\Win32-Release\jars\*"; DestDir: "{app}\jars"; Excludes: ".*"; Flags: recursesubdirs; Components: programfilesSource: "doc\release-doc\*"; DestDir: "{app}\docs"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
;Source: "doc\installed-doc\users\*"; DestDir: "{app}\docs"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\FileIOPlugin\*"; DestDir: "{app}\plugins\FileIOPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\MzXmlIOPlugin\*"; DestDir: "{app}\plugins\MzXmlIOPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\MzmlIOPlugin\*"; DestDir: "{app}\plugins\MzmlIOPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\AIAInputPlugin\*"; DestDir: "{app}\plugins\AIAInputPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\MsbIOPlugin\*"; DestDir: "{app}\plugins\MsbIOPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\TextIOPlugin\*"; DestDir: "{app}\plugins\TextIOPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\PngOutputPlugin\*"; DestDir: "{app}\plugins\PngOutputPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\ShimadzuLcmsInputPlugin\*"; DestDir: "{app}\plugins\ShimadzuLcmsInputPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
;Source: "build\msvs\stage\Win32-Release\plugins\AbQstarInputPlugin\*"; DestDir: "{app}\plugins\AbQstarInputPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
;Source: "build\msvs\stage\Win32-Release\plugins\WatersInputPlugin\*"; DestDir: "{app}\plugins\WatersInputPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
;Source: "build\msvs\stage\Win32-Release\plugins\ThermoIOPlugin\*"; DestDir: "{app}\plugins\ThermoIOPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
;Source: "build\msvs\stage\Win32-Release\plugins\AgilentInputPlugin\*"; DestDir: "{app}\plugins\AgilentInputPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\DbManagerPlugin\*"; DestDir: "{app}\plugins\DbManagerPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\SQLiteDbPlugin\*"; DestDir: "{app}\plugins\SQLiteDbPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\PostgreSQLPlugin\*"; DestDir: "{app}\plugins\PostgreSQLPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\HttpProxyPlugin\*"; DestDir: "{app}\plugins\HttpProxyPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\MdiChildFramePlugin\*"; DestDir: "{app}\plugins\MdiChildFramePlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\SpecChromWinModePlugin\*"; DestDir: "{app}\plugins\SpecChromWinModePlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\ProfileCanvasPlugin\*"; DestDir: "{app}\plugins\ProfileCanvasPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\OverlappingCanvasPlugin\*"; DestDir: "{app}\plugins\OverlappingCanvasPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\VerticalBoxCanvasPlugin\*"; DestDir: "{app}\plugins\VerticalBoxCanvasPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\ProfileDrawerPlugin\*"; DestDir: "{app}\plugins\ProfileDrawerPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\ScaleDrawerPlugin\*"; DestDir: "{app}\plugins\ScaleDrawerPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\ProfileRangePlugin\*"; DestDir: "{app}\plugins\ProfileRangePlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\DataMapCanvasPlugin\*"; DestDir: "{app}\plugins\DataMapCanvasPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\DataMapDrawerPlugin\*"; DestDir: "{app}\plugins\DataMapDrawerPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\Sample3DViewPlugin\*"; DestDir: "{app}\plugins\Sample3DViewPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\DataMapRangePlugin\*"; DestDir: "{app}\plugins\DataMapRangePlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\FirstViewPlugin\*"; DestDir: "{app}\plugins\FirstViewPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\SelectRangePlugin\*"; DestDir: "{app}\plugins\SelectRangePlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\MagnifyingViewPlugin\*"; DestDir: "{app}\plugins\MagnifyingViewPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\SpectrumChangerPlugin\*"; DestDir: "{app}\plugins\SpectrumChangerPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\SpectrumPrecursorProductChangerPlugin\*"; DestDir: "{app}\plugins\SpectrumPrecursorProductChangerPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\ChromatogramOperationPlugin\*"; DestDir: "{app}\plugins\ChromatogramOperationPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\SpectraSearchPlugin\*"; DestDir: "{app}\plugins\SpectraSearchPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\RawWaveformPlugin\*"; DestDir: "{app}\plugins\RawWaveformPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\PeakWaveformPlugin\*"; DestDir: "{app}\plugins\PeakWaveformPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\DataMapFilterPlugin\*"; DestDir: "{app}\plugins\DataMapFilterPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\SampleTreeViewPlugin\*"; DestDir: "{app}\plugins\SampleTreeViewPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\DetailsViewPlugin\*"; DestDir: "{app}\plugins\DetailsViewPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\XYDataViewPlugin\*"; DestDir: "{app}\plugins\XYDataViewPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\SpectraGridViewPlugin\*"; DestDir: "{app}\plugins\SpectraGridViewPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\LogViewPlugin\*"; DestDir: "{app}\plugins\LogViewPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\PeakPickingPlugin\*"; DestDir: "{app}\plugins\PeakPickingPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\CentroidModeLabelingPlugin\*"; DestDir: "{app}\plugins\CentroidModeLabelingPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\LocalMaximumLabelingPlugin\*"; DestDir: "{app}\plugins\LocalMaximumLabelingPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\MultiAlgorithmLabelingPlugin\*"; DestDir: "{app}\plugins\MultiAlgorithmLabelingPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\ApiPeakDetectionPlugin\*"; DestDir: "{app}\plugins\ApiPeakDetectionPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\SpecPeakFilterNumberOfPeaksPlugin\*"; DestDir: "{app}\plugins\SpecPeakFilterNumberOfPeaksPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\MRM3DPlugin\*"; DestDir: "{app}\plugins\MRM3DPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\DefaultChargePlugin\*"; DestDir: "{app}\plugins\DefaultChargePlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\SimpleChargeDetect2Plugin\*"; DestDir: "{app}\plugins\SimpleChargeDetect2Plugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\IntensityRateChargeDetectPlugin\*"; DestDir: "{app}\plugins\IntensityRateChargeDetectPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\PeakSearchPlugin\*"; DestDir: "{app}\plugins\PeakSearchPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\PeaksIOPlugin\*"; DestDir: "{app}\plugins\PeaksIOPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\PeakEditPlugin\*"; DestDir: "{app}\plugins\PeakEditPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
;Source: "build\msvs\stage\Win32-Release\plugins\MspeaksPlugin\*"; DestDir: "{app}\plugins\MspeaksPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
;Source: "build\msvs\stage\Win32-Release\plugins\MslowessPlugin\*"; DestDir: "{app}\plugins\MslowessPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\PeakLabelDrawerPlugin\*"; DestDir: "{app}\plugins\PeakLabelDrawerPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\MatrixViewPlugin\*"; DestDir: "{app}\plugins\MatrixViewPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\LabelingAndMergePeakPositionPlugin\*"; DestDir: "{app}\plugins\LabelingAndMergePeakPositionPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\MergedSpectrumPeaksPlugin\*"; DestDir: "{app}\plugins\MergedSpectrumPeaksPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\MrmPeakFinderPlugin\*"; DestDir: "{app}\plugins\MrmPeakFinderPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\PeakPositionsFileImportPlugin\*"; DestDir: "{app}\plugins\PeakPositionsFileImportPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\SimplePeakPositionAppenderPlugin\*"; DestDir: "{app}\plugins\SimplePeakPositionAppenderPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\NeighborPeaksPlugin\*"; DestDir: "{app}\plugins\NeighborPeaksPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\CommonsMathPlugin\*"; DestDir: "{app}\plugins\CommonsMathPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\UTestPlugin\*"; DestDir: "{app}\plugins\UTestPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\MatrixGroupPlotPlugin\*"; DestDir: "{app}\plugins\MatrixGroupPlotPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\MatrixOverlappingViewPlugin\*"; DestDir: "{app}\plugins\MatrixOverlappingViewPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\BasicScalingPlugin\*"; DestDir: "{app}\plugins\BasicScalingPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\MatrixPeaksMergePlugin\*"; DestDir: "{app}\plugins\MatrixPeaksMergePlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\IdentificationPlugin\*"; DestDir: "{app}\plugins\IdentificationPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\MascotSearchPlugin\*"; DestDir: "{app}\plugins\MascotSearchPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\MascotPMFForLCMSPlugin\*"; DestDir: "{app}\plugins\MascotPMFForLCMSPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\XTandemSearchPlugin\*"; DestDir: "{app}\plugins\XTandemSearchPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\opt\tandem\*"; DestDir: "{app}\opt\tandem"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\MassBankToolsPlugin\*"; DestDir: "{app}\plugins\MassBankToolsPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\CompoundDbPlugin\*"; DestDir: "{app}\plugins\CompoundDbPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\TppPlugin\*"; DestDir: "{app}\plugins\TppPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\BaselineSubtractPlugin\*"; DestDir: "{app}\plugins\BaselineSubtractPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\AbcBaselineDetectionPlugin\*"; DestDir: "{app}\plugins\AbcBaselineDetectionPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\LinearBaselineDetectionPlugin\*"; DestDir: "{app}\plugins\LinearBaselineDetectionPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\BackgroundSubtractPlugin\*"; DestDir: "{app}\plugins\BackgroundSubtractPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\FilterPlugin\*"; DestDir: "{app}\plugins\FilterPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\LinearFilterPlugin\*"; DestDir: "{app}\plugins\LinearFilterPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\WindowFilterPlugin\*"; DestDir: "{app}\plugins\WindowFilterPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\MzRangeFilterPlugin\*"; DestDir: "{app}\plugins\MzRangeFilterPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
;Source: "build\msvs\stage\Win32-Release\plugins\MsbackadjBaselineDetectionPlugin\*"; DestDir: "{app}\plugins\MsbackadjBaselineDetectionPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\OperationManagerPlugin\*"; DestDir: "{app}\plugins\OperationManagerPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\NormalizationPlugin\*"; DestDir: "{app}\plugins\NormalizationPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\AlignmentPlugin\*"; DestDir: "{app}\plugins\AlignmentPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\MergedSpectrumPlugin\*"; DestDir: "{app}\plugins\MergedSpectrumPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\CompositionPlugin\*"; DestDir: "{app}\plugins\CompositionPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\SampleMergePlugin\*"; DestDir: "{app}\plugins\SampleMergePlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\MsmsSpectraMergePlugin\*"; DestDir: "{app}\plugins\MsmsSpectraMergePlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\ResamplingPlugin\*"; DestDir: "{app}\plugins\ResamplingPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\GlycanAnalysisPlugin\*"; DestDir: "{app}\plugins\GlycanAnalysisPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\SampleTreeBuilder\*"; DestDir: "{app}\plugins\SampleTreeBuilder"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\ClipboardPlugin\*"; DestDir: "{app}\plugins\ClipboardPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\SettingsPlugin\*"; DestDir: "{app}\plugins\SettingsPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\HelpPlugin\*"; DestDir: "{app}\plugins\HelpPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\OperationLogPlugin\*"; DestDir: "{app}\plugins\OperationLogPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\PluginManagerPlugin\*"; DestDir: "{app}\plugins\PluginManagerPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\EditSampleUserProperties\*"; DestDir: "{app}\plugins\EditSampleUserProperties"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\BatchServicePlugin\*"; DestDir: "{app}\plugins\BatchServicePlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\CommandLineBatchServicePlugin\*"; DestDir: "{app}\plugins\CommandLineBatchServicePlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\ScriptPlugin\*"; DestDir: "{app}\plugins\ScriptPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\IronPythonPlugin\*"; DestDir: "{app}\plugins\IronPythonPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\SimcaDisplayPlugin\*"; DestDir: "{app}\plugins\SimcaDisplayPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\AB3DPlugin\*"; DestDir: "{app}\plugins\AB3DPlugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles
Source: "build\msvs\stage\Win32-Release\plugins\AB3D2Plugin\*"; DestDir: "{app}\plugins\AB3D2Plugin"; Excludes: ".*"; Flags: recursesubdirs; Components: programfiles



[InstallDelete]
Type: filesandordirs; Name: {app}\data
Type: filesandordirs; Name: {app}\docs
Type: filesandordirs; Name: {app}\plugins
Type: filesandordirs; Name: {app}\samples
Type: filesandordirs; Name: {group}

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"

[Icons]
Name: "{group}\Mass++"; Filename: "{app}\bin\mspp.exe"; IconFileName: "{app}\bin\mspp48x48.ico"; WorkingDir: "{app}\bin";
Name: "{group}\Release Notes"; Filename: "{app}\docs\ReleaseNotes.txt";
Name: "{group}\Mass++ User's Manual"; Filename: "{app}\docs\UserManual.pdf";
Name: "{userdesktop}\Mass++ (32-bit)"; Filename: "{app}\bin\mspp.exe"; IconFileName: "{app}\bin\mspp48x48.ico"; WorkingDir: "{app}\bin"; Tasks: desktopicon

[Registry]
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: string; ValueName: "MASSPP_HOME"; ValueData:"{app}"; Flags: uninsdeletevalue uninsdeletekeyifempty
Root: HKCR; Subkey: ".msb"; ValueType: string; ValueName: ""; ValueData: "Mass++2"; Flags: uninsdeletevalue uninsdeletekeyifempty
Root: HKCR; Subkey: "Mass++2"; ValueType: string; ValueName: ""; ValueData: ""; Flags: uninsdeletekey
Root: HKCR; Subkey: "Mass++2\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\bin\mspp32x32.ico"
Root: HKCR; Subkey: "Mass++2\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\bin\mspp.exe"" -in file ""%1"""
Root: HKLM; Subkey: "Software\Mass++2"; ValueType: string; ValueName: "InstallDir"; ValueData: "{app}"; Flags: uninsdeletevalue uninsdeletekeyifempty
Root: HKLM; Subkey: "Software\Mass++2"; ValueType: string; ValueName: "Version"; ValueData: "{#Version}"; Flags: uninsdeletevalue uninsdeletekeyifempty
Root: HKLM; Subkey: "Software\Mass++2"; ValueType: string; ValueName: "Platform"; ValueData: "{code:MyConst|Platform-Installer}"; Flags: uninsdeletevalue uninsdeletekeyifempty  
Root: HKLM; Subkey: "Software\Mass++2"; ValueType: string; ValueName: "InstallDate"; ValueData: "{code:MyConst|InstallDate}"
Root: HKLM; Subkey: "Software\Mass++2"; ValueType: string; ValueName: "MaxVersion"; ValueData: "{code:MyConst|MaxVer}"

[Code]
function URLDownloadToFile(lpUnknown: LongInt; szURL, szFileName: PChar; dwReserved, lpfnCB: LongInt): LongInt;
external 'URLDownloadToFileA@urlmon.dll stdcall setuponly';
 
function VersionNum(Ver: String): Integer;
    var Position: Integer;
    var Val: Integer;
    var Str0: String;
    var Str1: String;
 begin
    Str0 := Copy( Ver, 1, Length(Ver) );
    Val := 0;
    Position := Pos('.', Str0);

    while Position > 0 do
    begin
        Str1 := Copy( Str0, 1, Position - 1 );
        Val := Val * 1000;
        Val := Val + StrToInt( Str1 );
        Str0 := Copy( Str0, Position + 1, Length(Str0) - Position );
        Position := Pos('.', Str0 );
    end;

    Val := Val * 1000;
    if Length(Str0) > 0 then
    begin
       Val := Val + StrToInt(Str0);
    end;
    Result := Val;
end;


function MyConst(Param: String): String; 
    var thisVersion: String;
    var CHECK_KEY: String;
    var oldVersion: String;
    var installDate: String;
begin
    thisVersion := '{#Version}';
    CHECK_KEY := 'Software\Mass++2';


    if( RegValueExists(HKLM, CHECK_KEY, 'MaxVersion' ) ) then
        RegQueryStringValue(HKLM, CHECK_KEY, 'MaxVersion', oldVersion)
    else
        oldVersion := thisVersion;
  
    case Param of
      'Ver':    Result := thisVersion;
      'InstallDate':
      begin
        installDate := GetDateTimeString('yyyymmdd',  #0, #0)
        if(VersionNum(thisVersion) <= VersionNum(oldVersion)) then
          if(RegValueExists(HKLM, CHECK_KEY, 'InstallDate')) then
            RegQueryStringValue(HKLM, CHECK_KEY, 'InstallDate', installDate);

        Result:= installDate;
      end;

    'Platform-Installer':
      if Is64BitInstallMode then
        Result := 'x64'
      else
        Result := 'x86';

    'Exist-BitType':
      if Is64BitInstallMode then
        Result := 'x86'
      else
        Result := 'x64';

    'MaxVer':
      if(VersionNum(thisVersion)>VersionNum(oldVersion)) then
        Result := thisVersion
      else
        Result := oldVersion;
  end;
end;

function InstallCpp(url: String): Boolean;
var  
  Rslt: boolean;
  dRet: Cardinal;
  CHECK_KEY: String;

  begin
  Rslt := True;
  CHECK_KEY := 'Software\Microsoft\VisualStudio\10.0\VC\VCRedist\x86';
  if RegKeyExists(HKEY_LOCAL_MACHINE, CHECK_KEY) then
  begin
    if RegValueExists(HKEY_LOCAL_MACHINE, CHECK_KEY, 'Installed') then
    begin
      if RegQueryDWordValue(HKEY_LOCAL_MACHINE, CHECK_KEY, 'Installed', dRet) then
        begin
          if dRet = 1 then
          begin
                Rslt := False;
         end;
       end;
    end;
  end;

  if Rslt then
  begin
      UrlDownloadToFile(0, PChar(url), PChar(ExpandConstant('{tmp}') + '\' + ExtractFileName(url)), 0, 0);
  end;
  Result := Rslt;
end;

//function InstallCpp2008(url: String): Boolean;
//var  //  Rslt: boolean;
//  CHECK_KEY: String;
//
//  begin
//  Rslt := True;
//  CHECK_KEY := 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{9A25302D-30C0-39D9-BD6F-21E6EC160475}';
//  if RegKeyExists(HKEY_LOCAL_MACHINE, CHECK_KEY) then
//      Rslt := False;
//  if Rslt then
//  begin
//      UrlDownloadToFile(0, PChar(url), PChar(ExpandConstant('{tmp}') + '\vc9-' + ExtractFileName(url)), 0, 0);
//  end;
//  Result := Rslt;
//end;

function InstallFramework4(url: String): Boolean;
var  
  Rslt: boolean;
  dRet: Cardinal;
  CHECK_KEY: String;

  begin
  Rslt := True;
  CHECK_KEY := 'Software\Microsoft\NET Framework Setup\NDP\v4\Full';
  if RegKeyExists(HKEY_LOCAL_MACHINE, CHECK_KEY) then
  begin
    if RegValueExists(HKEY_LOCAL_MACHINE, CHECK_KEY, 'Install') then
    begin
      if RegQueryDWordValue(HKEY_LOCAL_MACHINE, CHECK_KEY, 'Install', dRet) then
        begin
          if dRet = 1 then
          begin
                Rslt := False;
         end;
       end;
    end;
  end;

  if Rslt then
  begin
      UrlDownloadToFile(0, PChar(url), PChar(ExpandConstant('{tmp}') + '\' + ExtractFileName(url)), 0, 0);
  end;
  Result := Rslt;
end;


procedure DownloadFile(url: String);
begin
    UrlDownloadToFile(0, PChar(url), PChar(ExpandConstant('{tmp}') + '\' + ExtractFileName(url)), 0, 0);
end;


function InitializeSetup(): Boolean;
var
Msg: String;
VerInstalled: String;
bRet:Boolean;
currNum:Integer;
installedNum:Integer;

begin

  if RegKeyExists(HKCR, 'Mass++') then
  begin
      Msg := 'The old version of Mass++ is installed. Please uninstall it before installing Mass++2';
      MsgBox(Msg, mbError, mb_Ok);
  end else
  begin
      bRet := RegQueryStringValue(HKLM, 'SOFTWARE\Mass++2', 'Version', VerInstalled);
      currNum := VersionNum( '{#Version}' );
      installedNum := VersionNum( VerInstalled );

      if bRet and ( currNum < installedNum ) then
      begin
        Msg := 'The newer Mass++ is already installed.';
        MsgBox(Msg, mbError, mb_Ok);
        Result := False;
      end else
      begin
        Result := True;
      end;
  end;
end;


function ShouldSkipPage(PageID: Integer): Boolean;
var
VerInstalled: String;
bRet:Boolean;

begin

  bRet := RegQueryStringValue(HKLM, 'SOFTWARE\Mass++2', 'Version', VerInstalled);

  if bRet and ( MyConst('Ver') >= VerInstalled) then
  begin
    case PageID of
      wpLicense:
        Result := False;
      wpSelectDir:
        if ( MyConst('Ver') = VerInstalled) then begin
          Result := True;
        end else
          Result := False;
      wpSelectProgramGroup:
        Result := True;
       wpSelectTasks:
        Result := True;
    else
      Result := False;
    end;
  end else
    Result := False;
end;

function SendMessageTimeoutA(
    hWnd:       Longint;
    msg:        Longint;
    wParam:     Longint;
    lParam:     String;
    fuFlag:     Longint;
    uTimeout:   Longint;
    lpdwResult: Longint
): Longint;
external 'SendMessageTimeoutA@user32.dll stdcall';


procedure addPath( path: String );
var
  paths: String;
begin
  RegQueryStringValue( HKLM, 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment', 'Path', paths );
  
  if Pos( path, paths ) <> 0 then begin
    Exit;
  end
  if Length( paths ) = 0 then begin
    paths := path;
  end else begin
    paths := paths + ';' + path;
  end
  
  RegWriteStringValue( HKLM, 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment', 'Path', paths );

  SendMessageTimeoutA( $ffff, $1a, 0, 'Environment', $2, 5000, 0 );
end;


procedure removePath( path: String );
var
  paths: String;
  s:     String;
  p:     Longint;
begin
  RegQueryStringValue( HKLM, 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment', 'Path', paths );
  
  s := ';' + path;
  p:= Pos( s, paths );
  if p = 0 then begin
    s := path;
    p := Pos( s, paths );
  end
  
  if p = 0 then begin
    Exit;
  end
  
  Delete( paths, p, Length( s ) )
  
  RegWriteStringValue( HKLM, 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment', 'Path', paths );

  SendMessageTimeoutA( $ffff, $1a, 0, 'Environment', $1, 5000, 0 );
end;


procedure CurStepChanged( CurStep: TSetupStep );
var
  path: String;
begin
  if CurStep = ssDone then begin
    path := ExpandConstant( '{app}\bin' );
    addPath( path );
  end
end;

procedure CurUninstallStepChanged( CurUninstallStep: TUninstallStep );
var
  path: String;
begin
  if CurUninstallStep = usDone then begin
    path := ExpandConstant( '{app}\bin' );
    removePath( path );
  end
end;

function GetDefaultDir(def: string): string;
var
   tmp : string;
begin
   tmp := ExpandConstant('{pf}') + '\Mass++';

   if RegQueryStringValue(HKEY_LOCAL_MACHINE, 'Software\Mass++2', 'InstallDir', tmp ) then
   begin
   end;
   Result := tmp;
end;


[Run]
;Filename: "{tmp}\vcredist_x86.exe"; StatusMsg: "Installing Visual C++ 2010 runtime ..."; Check: InstallCpp('http://download.microsoft.com/download/5/B/C/5BC5DBB3-652D-4DCE-B14A-475AB85EEF6E/vcredist_x86.exe');
Filename: "{tmp}\vcredist_x86.exe"; StatusMsg: "Installing Visual C++ 2010 runtime ..."; Check: InstallCpp('http://download.microsoft.com/download/1/6/5/165255E7-1014-4D0A-B094-B6A430A6BFFC/vcredist_x86.exe');
;Filename: "{tmp}\vc9-vcredist_x86.exe"; StatusMsg: "Installing Visual C++ 2008 runtime ..."; Check: InstallCpp2008('http://download.microsoft.com/download/d/d/9/dd9a82d0-52ef-40db-8dab-795376989c03/vcredist_x86.exe');
;Filename: "{tmp}\vc9-vcredist_x86.exe"; StatusMsg: "Installing Visual C++ 2008 runtime ..."; Check: InstallCpp2008('http://download.microsoft.com/download/1/b/2/1b2312a1-b2e5-479c-9103-d9616e6761d9/vcredist_x86.exe');
Filename: "{tmp}\dotNetFx40_Full_x86_x64.exe"; StatusMsg: "Installing Framework4 runtime ..."; Check: InstallFramework4('http://download.microsoft.com/download/9/5/A/95A9616B-7A37-4AF6-BC36-D6EA96C8DAAE/dotNetFx40_Full_x86_x64.exe');
