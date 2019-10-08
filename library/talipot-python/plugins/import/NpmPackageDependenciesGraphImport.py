# Copyright (C) 2019  The Talipot developers
#
# Talipot is a fork of Tulip, created by David Auber
# and the Tulip development Team from LaBRI, University of Bordeaux
#
# See the AUTHORS file at the top-level directory of this distribution
# License: GNU General Public License version 3, or any later version
# See top-level LICENSE file for more information

# Talipot import plugin written in Python for importing the dependencies
# graph of a npm package (node package manager).
# The plugin simply parses the package.json files to extract the whole
# dependencies graph of a package. Be sure to have called 'npm install in
# the package directory first in order to get the complete dependencies graph

from talipot import tlp
import talipotplugins

import json
import os

npmPackageDirParamName = "npm package dir"

packageNode = {}


def parsePackageJsonForDependenciesGraph(npmPackageDir, graph,
                                         packageName=None):

    # get package.json file path
    if not packageName:
        # no package name provided as parameter, it is the root package
        packageJsonFilePath = '%s/package.json' % npmPackageDir
    else:
        # dependency package from the node_modules directory
        packageJsonFilePath = '%s/node_modules/%s/package.json' % (
            npmPackageDir, packageName)

    # package.json does not exist, abort
    if not os.path.exists(packageJsonFilePath):
        return

    # parse package.json file
    with open(packageJsonFilePath) as packageJson:
        packageInfos = json.load(packageJson)

    # create a string property that will store dependency type on edges
    dependencyType = graph.getStringProperty('dependencyType')

    # create the top level package node (entry point)
    if not packageName:
        packageName = packageInfos['name']
        packageNode[packageName] = graph.addNode()
        graph['viewLabel'][packageNode[packageName]] = packageName

    currentPackageNode = packageNode[packageName]

    # iterate over package dependencies
    for depType in ('dependencies', 'peerDependencies', 'devDependencies'):
        if depType in packageInfos:
            for dep in packageInfos[depType]:
                # create a node associated to the package if it does not
                # exist yet
                if dep not in packageNode:
                    packageNode[dep] = graph.addNode()
                    graph['viewLabel'][packageNode[dep]] = dep
                    # parse dependency package dependencies
                    parsePackageJsonForDependenciesGraph(
                        npmPackageDir, graph, dep)
                    parsePackageJsonForDependenciesGraph(
                        '%s/node_modules/%s' % (npmPackageDir, packageName),
                        graph, dep)
                # add an edge between the package and its dependency if it has
                # not already be created
                if not graph.hasEdge(currentPackageNode, packageNode[dep]):
                    depEdge = graph.addEdge(currentPackageNode,
                                            packageNode[dep])
                    dependencyType[depEdge] = depType


class NpmPackageDependenciesGraphImport(tlp.ImportModule):

    def __init__(self, context):
        tlp.ImportModule.__init__(self, context)
        self.addDirectoryParameter(
            npmPackageDirParamName, 'The root directory of the npm package')

    def icon(self):
        return '%s/npm.png' % os.path.dirname(__file__)

    def importGraph(self):
        npmPackageDir = self.dataSet[npmPackageDirParamName]
        # Check if the directory contains a npm package first
        rootPackageJson = '%s/package.json' % npmPackageDir
        if not os.path.exists(rootPackageJson):
            if self.pluginProgress:
                self.pluginProgress.setError(
                    ('Error : directory %s does not seem to contain a '
                     'npm package.') % npmPackageDir)
            return False

        # parse package dependencies graph
        parsePackageJsonForDependenciesGraph(npmPackageDir, self.graph)

        # apply a force directed algorithm to draw the graph
        self.graph.applyLayoutAlgorithm(
            'Fast Multipole Multilevel Embedder (OGDF)')

        return True


pluginDoc = """
Import the packages dependencies graph from a npm package.
Be sure to have called 'npm install' in the package directory first
in order to get the complete dependencies graph.
"""

# The line below does the magic to register the plugin to the plugin database
# and updates the GUI to make it accessible through the menus.
talipotplugins.registerPluginOfGroup(
    'NpmPackageDependenciesGraphImport', 'Npm package dependencies graph',
    'Antoine Lambert', '26/11/2015',
    pluginDoc, '1.0', 'Software')
