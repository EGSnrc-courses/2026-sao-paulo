#!/usr/bin/python

import os

# =================================================================================================
# define parameters (which become command-line options)
# =================================================================================================
def set():

    param = dict()

    # EGSnrc application
    param['template']           = 'Lab-19-methane.template'    # application input template
    param['label']              = '_%(windowMaterial)s-%(windowThickness).4fcm'   # add this label to generated user files

    # run control
    param['n_case']             = '1e5'                         # number of histories to run
    param['accuracy_sought']    = 0.00001                       # statistical accuracy sought
    param['geom_error_limit']   = 1000                          # maximum number of geometry errors
    param['n_chunk']            = 10                            # number of chunks for parallel simulations
    param['n_batch']            = 10                            # number of batches per chunk

    # thresholds
    param['ae']                 = 0.512
    param['ap']                 = 0.001
    param['ue']                 = 5.511 # Watch out for these energy maximums
    param['up']                 = 5.0

    # cutoffs
    param['ecut']               = 0.512
    param['pcut']               = 0.001

    # simulation
    param['windowThickness']    = [0.001, 0.0005] # window thickness in cm
    param['windowMaterial']     = ['copper', 'aluminum'] # window materials, make sure these are all defined

    # system paths
    param ['user_path']         = './'
    #param ['user_path']         = os.path.join(os.getenv('EGS_HOME'), '') # Just an example that uses environment variables

    return param

# =================================================================================================
# define other computed parameters
# =================================================================================================
def compute(param):

    param['phsp1_filename']  = (os.path.splitext(param['template'])[0] + param['label']) % param + "-phsp1"
    param['phsp2_filename']  = (os.path.splitext(param['template'])[0] + param['label']) % param + "-phsp2"
    param['spectrum_filename'] = (os.path.splitext(param['template'])[0] + param['label']) % param + "-spectrum.dat"

    if param['windowMaterial'] == 'copper':
        param['n_case'] = '1e6'

    return param
