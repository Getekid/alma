/**
 * 
 * Class extending the functionality of the Clustering class from the BSc Thesis of Dimitrios - Nikolaos Kostantakis
 *   Code style has followed the original one.
 *   Most methods are variations of the original ones, usually simply adjusted to different inputs.
 * 
 * @author Thomas Pappas
 * @see Dimitrios - Nikolaos Konstantakis, Geometrical Road Segmentation and Clustering, BSc Thesis
 *   National and Kapodistrian University of Athens, Department of Informatics and Telecommunications, 2018 April
 */
package osm.segmentation.clustering;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import Jama.Matrix;
import Jama.SingularValueDecomposition;
import osm.segmentation.Bucket;
import osm.segmentation.Segment;
import osm.segmentation.curves.SegmentPoint;

public class Clustering2 extends Clustering {

	public Clustering2(Bucket bucket, int k) {
		super(bucket, k);
	}
	
	public double cRMSD_distance(List<SegmentPoint> curveA, List<SegmentPoint> curveB) {
		Matrix Q, U, VTransposed;

		Matrix matrixX = new Matrix(get_normalized_matrix_from_curve(curveA));
		Matrix matrixY = new Matrix(get_normalized_matrix_from_curve(curveB));

		SingularValueDecomposition svd = new SingularValueDecomposition((matrixX.transpose()).times(matrixY));
		// singular = svd.getSingularValues(); //double[] singular;

		U = svd.getU();
		VTransposed = (svd.getV()).transpose();

		Q = U.times(VTransposed);
		if (Q.det() < 0) {
			for (int i = 0; i < U.getRowDimension(); i++)
				U.set(i, 2, U.get(i, 2) * (-1));
			Q = U.times(VTransposed);
		}

		return (((matrixX.times(Q)).minus(matrixY)).normF() / Math.sqrt(matrixX.getRowDimension()));
	}

	public double[][] get_normalized_matrix_from_curve(List<SegmentPoint> curveA) {
		double meanX = 0, meanY = 0, meanZ = 0;
		double[][] matrix = new double[curveA.size()][3];

		if (curveA.isEmpty())
			return null;

		for (int i = 0; i < curveA.size(); i++) {
			meanX += curveA.get(i).getCartesianX();
			meanY += curveA.get(i).getCartesianY();
			meanZ += curveA.get(i).getCartesianZ();
		}

		meanX /= curveA.size();
		meanY /= curveA.size();
		meanZ /= curveA.size();

		for (int i = 0; i < curveA.size(); i++) {
			matrix[i][0] = curveA.get(i).getCartesianX() - meanX;
			matrix[i][1] = curveA.get(i).getCartesianY() - meanY;
			matrix[i][2] = curveA.get(i).getCartesianZ() - meanZ;
		}

		return matrix;
	}
	
	public double polygonal_chains_distance_chain_translation(List<SimplePoint> chainA, List<SimplePoint> chainB) {
		SimplePoint centerA = this.get_polynogal_chain_center(chainA);
		SimplePoint centerB = this.get_polynogal_chain_center(chainB);

		// (1) Translate chainB so as centerA and centerB coincide
		this.polynogal_chain_translation(chainB, centerB, centerA);

		// (2) Rotate chainB
		// this.polynogal_chain_rotation(chainA, chainB, centerA);

		// (3) Compute Discrete Frechet
		double distance = this.compute_discrete_Frechet(chainA, chainB);

		return distance;
	}
	
	public double polygonal_chains_distance_chain(List<SimplePoint> chainA, List<SimplePoint> chainB) {
		SimplePoint centerA = this.get_polynogal_chain_center(chainA);
		SimplePoint centerB = this.get_polynogal_chain_center(chainB);

		// (1) Translate chainB so as centerA and centerB coincide
		this.polynogal_chain_translation(chainB, centerB, centerA);

		// (2) Rotate chainB
		this.polynogal_chain_rotation(chainA, chainB, centerA);

		// (3) Compute Discrete Frechet
		double distance = this.compute_discrete_Frechet(chainA, chainB);

		return distance;
	}
	
	public List<SimplePoint> convert_segment_to_polygonal_chain(Segment segment) {
		if (segment == null)
			return null;
		List<SimplePoint> chain = new ArrayList<SimplePoint>();

		Iterator<SegmentPoint> iterator = segment.getCurve().iterator();
		while (iterator.hasNext()) {
			SegmentPoint segmentpoint = iterator.next();
			SimplePoint point = new SimplePoint(segmentpoint.getCartesianX(), segmentpoint.getCartesianY(),
					segmentpoint.getCartesianZ());
			chain.add(point);
		}

		return chain;
	}

}
