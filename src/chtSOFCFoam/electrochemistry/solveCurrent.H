{
	Info << nl << "Solve current" << endl;

	for (int counter1=1; counter1<=numCells; counter1++)
	{
		Info << nl << "layer " << counter1 << endl;

		char rLayer[16]; // string which will contain the number

		sprintf (rLayer, "layer%d", counter1 ); // %d makes the result be a decimal integer

		label layerId = electrolyteMesh->cellZones().findZoneID(rLayer);
		labelList layerCell(electrolyteMesh->cellZones()[layerId]);

		{
		    dimensionedScalar jbar ("jbar", dimensionSet(0, -2, 0, 0, 0, 1, 0), 0.0);

		    jbar.value() = 0.0;
		    scalar jDensityDV = 0.0;
		    scalar volume	= 0.0;
		    scalar sumVolume = 0.0;
		    //scalar counter2 = 0.0;

		    forAll(layerCell, j)
		    {    		    
			volume = mag(electrolyteMesh->V()[layerCell[j]]);
			jDensityDV += jDensityIn[layerCell[j]]*volume;
			sumVolume += volume;
			//counter2 += 1;
			V.value() = voltage[layerCell[j]];
		    }

			reduce(sumVolume,plusOp<scalar>());
			reduce(jDensityDV,plusOp<scalar>());
			//reduce(counter2,plusOp<scalar>());


		    jbar.value() = jDensityDV/sumVolume;
//		    Info << nl << "sumVolume = " << sumVolume << endl;
//		    Info << nl << "jDensityDV" << jDensityDV << endl;
//		    Info << nl << "counter2" << counter2 << endl;


		    Info << "min,max(Nernst): "
			 << Foam::min(Nernst) <<" , "
			 << Foam::max(Nernst) << endl;

//		    Info << "min,mean,max(i): "
//			 << Foam::min(idensityIn) <<" , " << jbar.value() << " , "
//			 << Foam::max(idensityIn) << endl;

		    // voltage correction
		    if (galvanostatic)
		    {
				V += Rhat*(jbar - jbar0);

				Info << "jbar0 = " << jbar0.value()
					<< "    jbar = " << jbar.value()
					<< "    V = " << V.value() << endl;

				forAll(layerCell, j)
				{
				voltage[layerCell[j]] = V.value();
				}
		    }
		    else
		    {
			Info << "    jbar = " << jbar.value()
			     << "    V = " << V.value() << endl;
		    }
		}
	}
}






