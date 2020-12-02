/**
 * 
 * This class is an adjustment of the original Main class from the BSc Thesis of Dimitrios - Nikolaos Kostantakis
 *   Code style has followed the original one.
 *   Custom code includes the 'bucketLookup' method plus the 'Custom code' block in the main one.
 * 
 * @author Thomas Pappas
 * @see Dimitrios - Nikolaos Konstantakis, Geometrical Road Segmentation and Clustering, BSc Thesis
 *   National and Kapodistrian University of Athens, Department of Informatics and Telecommunications, 2018 April
 */
package osm;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.xml.sax.SAXException;

import info.pavie.basicosmparser.model.Element;
import osm.segmentation.Bucket;
import osm.segmentation.OsmParser;
import osm.segmentation.Segment;
//import osm.segmentation.SegmentationSystem;
import osm.segmentation.SegmentationSystem2;
import osm.segmentation.clustering.Clustering2;
import osm.segmentation.clustering.SimplePoint;

public class Main {
	/**
	 * For a given bucket, find the two highways with the same max size.
	 * 
	 * @param bucket
	 * @return
	 *   The integer?
	 */
	public static List<Segment> bucketLookup(Bucket bucket) {
		// Initialise an empty Segment list to store the max same size buckets.
		List<Segment> bucketsMaxSameSize = new ArrayList<Segment>();
		List<Segment> highways = bucket.getHighways();
		List<Integer> sizes = new ArrayList<Integer>();
		
		// Sort the sizes (descending) and look for the max value that occurs at least twice.
		for (Iterator<Segment> iterator = bucket.getHighways().iterator(); iterator.hasNext();) {
			Segment highway = iterator.next();
			sizes.add(highway.getCurve().size());
		}
		List<Integer> sizesOrd = new ArrayList<Integer>(sizes);
		Collections.sort(sizesOrd, Collections.reverseOrder());
		int maxSameSize = sizesOrd.get(0);
		Boolean found = false;
		for (int i = 1; i < sizesOrd.size(); i++) {
			if (sizesOrd.get(i) == maxSameSize) {
				found = true;
				break;
			}
			maxSameSize = sizesOrd.get(i);
		}

		// If such highways are found, store them.
		if (found) {
			Integer maxHighwayIndex = sizes.indexOf(maxSameSize);
			bucketsMaxSameSize.add(highways.get(maxHighwayIndex));
			sizes.remove(maxHighwayIndex.intValue());
			maxHighwayIndex = sizes.indexOf(maxSameSize) + 1;
			bucketsMaxSameSize.add(highways.get(maxHighwayIndex));
		}
		
		return bucketsMaxSameSize;
	}
	
	public static void main(String args[]) {
		
		File osmFile = new File("");	//Create a file object for your OSM XML file
		OsmParser parser = new OsmParser();
				
		try {
			Map<String,Element> result = parser.parseFile(osmFile);			//Parse OSM data, and put result in a Map object
			SegmentationSystem2 segmentationSystem = new SegmentationSystem2(result);
			segmentationSystem.menu();
			
			/**
			 * Custom code start.
			 */
			// Initialise a cluster object to later access the distance calculation functions.
			Map<String, Double> elevationTable = new HashMap<String, Double>();
			Bucket bucketDummy = new Bucket("unclassified", elevationTable);
			Clustering2 cluster = new Clustering2(bucketDummy, 0);
			
			// Print the header.
			System.out.println("Highway type,|A|,|B|,dFt,dFm,c-RMSD");
			
			// Iterate through all buckets and calculate different distance measures.
			for (Map.Entry<String, Bucket> entry : segmentationSystem.getHashtable().entrySet()) {
				Bucket bucket = entry.getValue();
				if (bucket.getHighways().isEmpty()) continue;
				List<Segment> bucketsMaxSameSize = Main.bucketLookup(bucket);
				Segment highwayA = bucketsMaxSameSize.get(0);
				Segment highwayB = bucketsMaxSameSize.get(1);
				List<SimplePoint> chainA = cluster.convert_segment_to_polygonal_chain(highwayA);
				List<SimplePoint> chainB = cluster.convert_segment_to_polygonal_chain(highwayB);

				// Calculate Frechet distance with translation, with translation and rotation,
				// as well as the c-RMSD of the two chains.
				double dFt = cluster.polygonal_chains_distance_chain_translation(chainA, chainB);
				double dFm = cluster.polygonal_chains_distance_chain(chainA, chainB);
				double cRMSD = cluster.cRMSD_distance(highwayA.getCurve(), highwayB.getCurve());

				// Print the data.
				System.out.println(bucket.getRoadtype() + "," + chainA.size() + "," + chainB.size() + "," + dFt + "," + dFm + "," + cRMSD);
				// Print for a LaTeX table.
//				System.out.println(bucket.getRoadtype() + " & $" + chainA.size() + "$ & $" + chainB.size() + "$ & $" + dFt + "$ & $" + dFm + "$ & $" + cRMSD + "$\\\\");
			}
			/**
			 * Custom code end.
			 */

//			cluster.compare_dfs();
		} catch (IOException | SAXException e) {
			((Throwable) e).printStackTrace();								//Input/output errors management
		}
		
	}
}
