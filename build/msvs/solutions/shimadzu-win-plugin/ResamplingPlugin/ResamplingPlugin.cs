/// <summary>
/// [FILE] ResamplingPlugin.cs
/// [ABSTRACT] Resampling Plugin Mass++ access class
/// Copyright (C) 2013-05-31 Shimadzu
/// </summary>

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

// Mass++ namespace
using kome.clr;
using ResamplingPlugin.SpectrumCalculation;
using ResamplingPlugin.RemoveContaminantPeak;

namespace ResamplingPlugin
{
    public class ResamplingPlugin
    {
        /// <summary>
        /// Calcualte spectrums.
        /// </summary>
        /// <param name="clrParams">parameters for C# plug-in from Mass++</param>
        /// <returns></returns>
        public ClrVariant SpectrumCaluculation(ClrParameters clrParams)
        {
            // Return value (Success: true, Failure: false)
            ClrVariant ret = new ClrVariant();
            // We initially set ret.prim.boolVal to false so that we can return a false
            // result easily using "return ret" if a failure condition occurs.
            ret.type = ClrVariant.DataType.BOOL;
            ret.obj = false;

            // Convert clrParams to ActiveObject.
            ClrVariant clrVar = ClrPluginCallTool.getActiveObject(clrParams);

            // Display main window.
            SpectrumCalculationManager.DisplayDlgSpecCalc(clrVar);

            ret.obj = true;
            return ret;
        }

        /// <summary>
        /// Remove contaminant peak is Enabled.
        /// </summary>
        /// <param name="clrParams">parameters for C# plug-in from Mass++</param>
        /// <returns></returns>
        public ClrVariant RemoveContaminantPeak(ClrParameters clrParams)
        {
            // Return value (Success: true, Failure: false)
            ClrVariant ret = new ClrVariant();
            // We initially set ret.prim.boolVal to false so that we can return a false
            // result easily using "return ret" if a failure condition occurs.
            ret.type = ClrVariant.DataType.BOOL;
            ret.obj = false;

            // Convert clrParams to ActiveObject
            ClrVariant clrVar = ClrPluginCallTool.getActiveObject(clrParams);

            // Display main window.
            RemoveContaminantPeakManager.DisplayDlgRemovContamiPeak(clrVar);

            ret.obj = true;
            return ret;
        }

        /// <summary>
        /// Return Remove contaminant peak is Enabled.
        /// </summary>
        /// <param name="clrParams"></param>
        /// <returns></returns>
        public ClrVariant RemoveContaminantPeakIsEnabled(ClrParameters clrParams)
        {
            ClrVariant ret = new ClrVariant();
            ret.type = ClrVariant.DataType.BOOL;
            ret.obj = false;

            try
            {
                ClrMsDataVariant msObj = new ClrMsDataVariant(ClrPluginCallTool.getActiveObject(clrParams));
                SampleWrapper sw = msObj.getSample();
                if (sw != null)
                {
                    DataGroupNodeWrapper dgnw = sw.getRootDataGroupNode();
                    if (1 < dgnw.getNumberOfSpectra())
                    {
                        ret.obj = true;
                    }
                }
            }
            catch
            {
                ret.obj = false;
            }
            return ret;
        }

        /// <summary>
        /// Batch Resampling Entry Function.
        /// </summary>
        /// <param name="clrParams"></param>
        /// <returns></returns>
        public ClrVariant BatResample(ClrParameters clrParams)
        {
            // Return value (Success: true, Failure: false)
            ClrVariant ret = new ClrVariant();
            // We initially set ret.prim.boolVal to false so that we can return a false
            // result easily using "return ret" if a failure condition occurs.
            ret.type = ClrVariant.DataType.BOOL;
            ret.obj = false;

            // Call Manager
            int retValue = SpectrumCalculationManager.CommandlineSpecCalc(clrParams);

            if (retValue == 0)
            {
                ret.obj = true;     // Success.
            }
            else
            {
                ret.obj = false;    // Failure.
            }

            return ret;

        }

        /// <summary>
        /// Define of Firstspec option.
        /// (This function is not called normally. Please reference design document Page 37.)
        /// </summary>
        /// <param name="clrParams"></param>
        /// <returns></returns>
        public ClrVariant BatPitchFirstspec(ClrParameters clrParams)
        {
            // Return value (Success: true, Failure: false)
            ClrVariant ret = new ClrVariant();
            // We initially set ret.prim.boolVal to false so that we can return a false
            // result easily using "return ret" if a failure condition occurs.
            ret.type = ClrVariant.DataType.BOOL;
            ret.obj = false;
            
            return ret;
        }

        /// <summary>
        /// Define of Minpitch option.
        /// (This function is not called normally. Please reference design document Page 37.)
        /// </summary>
        /// <param name="clrParams"></param>
        /// <returns></returns>
        public ClrVariant BatPitchMinpitch(ClrParameters clrParams)
        {
            // Return value (Success: true, Failure: false)
            ClrVariant ret = new ClrVariant();
            // We initially set ret.prim.boolVal to false so that we can return a false
            // result easily using "return ret" if a failure condition occurs.
            ret.type = ClrVariant.DataType.BOOL;
            ret.obj = false;

            return ret;
        }

        /// <summary>
        /// Define of Average option.
        /// (This function is not called normally. Please reference design document Page 37.)
        /// </summary>
        /// <param name="clrParams"></param>
        /// <returns></returns>
        public ClrVariant BatPitchAverage(ClrParameters clrParams)
        {
            // Return value (Success: true, Failure: false)
            ClrVariant ret = new ClrVariant();
            // We initially set ret.prim.boolVal to false so that we can return a false
            // result easily using "return ret" if a failure condition occurs.
            ret.type = ClrVariant.DataType.BOOL;
            ret.obj = false;

            return ret;
        }

        /// <summary>
        /// Define of Specified option.
        /// (This function is not called normally. Please reference design document Page 37.)
        /// </summary>
        /// <param name="clrParams"></param>
        /// <returns></returns>
        public ClrVariant BatPitchSpecified(ClrParameters clrParams)
        {
            // Return value (Success: true, Failure: false)
            ClrVariant ret = new ClrVariant();
            // We initially set ret.prim.boolVal to false so that we can return a false
            // result easily using "return ret" if a failure condition occurs.
            ret.type = ClrVariant.DataType.BOOL;
            ret.obj = false;

            return ret;
        }
    }
}
