from pathlib import Path
from htmd.ui import *
import numpy as np


def get_real_binding_site_geo_center(scpdb_id):
    # Calculate the distance of the predicted point to the real binding site.
    local_path_to_scpdb = '/.../Project/scPDB/'
    mol = Molecule(local_path_to_scpdb + scpdb_id + '/site.mol2')
    coords = mol.get('coords')
    return np.mean(coords, axis=0)


def check_deepsite(scpdb_id, prediction):
    return np.linalg.norm(prediction - get_real_binding_site_geo_center(scpdb_id))


def check_fpocket(scpdb_id):
    pdb_id = scpdb_id.split('_')[0]

    # In case we need to iterate though all pockets.
    pathlist = Path('/.../Project/PD/'
                    + pdb_id + '/' + pdb_id + '_out/pockets').glob('*.pdb')
    for path in pathlist:
        mol = Molecule(str(path))
        coords = mol.get('coords')
        c = np.mean(coords, axis=0)
        d = np.linalg.norm(c - get_real_binding_site_geo_center(scpdb_id))
        if d < 20:
            print('Path', path, 'Center', c, 'distance', d)

    # Parse the PDB data
    mol = Molecule('/.../Project/PD/'
                   + pdb_id + '/' + pdb_id + '_out/pockets/pocket1_atm.pdb')
    coords = mol.get('coords')
    c = np.mean(coords, axis=0)
    return [c, np.linalg.norm(c - get_real_binding_site_geo_center(scpdb_id))]


# DeepSite
scpdb = '4ylh_11'
point = np.array([13.12000084, 45.86599922, 106.54499817])

# real = get_real_binding_site_geo_center(scpdb)
# print('(' + str(round(real[0], 2)) + ', ' + str(round(real[1], 2)) + ', ' + str(round(real[2], 2)) + ')')
# print(scpdb.split('_')[0].upper(), '&', scpdb,
#       '$(' + str(round(point[0], 2)) + ', ' + str(round(point[1], 2)) + ', ' + str(round(point[2], 2)) + ')$',
#       '&', '$' + str(check_deepsite(scpdb, point)) + '$', '\\\\')
# fpocket
# fp = check_fpocket('4ylh_11')
# print('Center', '(' + str(round(fp[0][0], 2)) + ', ' + str(round(fp[0][1], 2)) + ', ' + str(round(fp[0][2], 2)) + ')',
#       'distance', round(fp[1], 2))
