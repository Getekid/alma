/**
 * 
 * Class extending the functionality of the SegmentationSystem class from the BSc Thesis of Dimitrios - Nikolaos Kostantakis
 *   Code style has followed the original one.
 *   Most methods are variations of the original ones, usually simply adjusted to different inputs.
 * 
 * @author Thomas Pappas
 * @see Dimitrios - Nikolaos Konstantakis, Geometrical Road Segmentation and Clustering, BSc Thesis
 *   National and Kapodistrian University of Athens, Department of Informatics and Telecommunications, 2018 April
 */
package osm.segmentation;

import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import info.pavie.basicosmparser.model.Element;

public class SegmentationSystem2 extends SegmentationSystem {

	public SegmentationSystem2(Map<String, Element> osmResult) {
		super(osmResult);
	}

	public void menu() {
		super.menu();
		
		//Print the mean length from each bucket.
		for (Map.Entry<String, Bucket> entry : this.getHashtable().entrySet()) {
			Bucket bucket = entry.getValue();
			System.out.println("Mean length: "+bucket.getMeanlength()+"\t for Bucket \""+bucket.getRoadtype()+"\"");
		}
	}


	public void segmentation(Bucket bucket) {
		for (Iterator<Segment> iterator = bucket.getHighways().iterator(); iterator.hasNext();) {
			Segment highway = iterator.next();
			this.segmentationByCurvature(highway);
		}
	}
	
	public void createDataset() throws FileNotFoundException, UnsupportedEncodingException {
		PrintWriter writer = new PrintWriter("", "UTF-8");	
		for (Map.Entry<String, Bucket> entry : this.getHashtable().entrySet()) {
			Bucket bucket = entry.getValue();
			
			List<Segment> segments = bucket.getSegments();
			Iterator<Segment> iterator = segments.iterator();
			while(iterator.hasNext()) {
				Segment segment = iterator.next();
				writer.print(segment.getId()+"\t"+segment.getWay().getId()+"\t"+segment.getCurve().size()+"\t");
				
				for(int n=0; n<segment.getCurve().size(); n++) {
					writer.print("("+segment.getCurve().get(n).getCartesianX()+", "+segment.getCurve().get(n).getCartesianY()+", "+segment.getCurve().get(n).getCartesianZ()+")");
					if(n==segment.getCurve().size()-1) continue;
					writer.print(", ");
				}
				writer.println();
			}
		}	
		writer.close();	
	}
}
