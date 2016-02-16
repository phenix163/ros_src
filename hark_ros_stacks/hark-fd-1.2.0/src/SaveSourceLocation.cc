/*
 * Copyright 2008 Kyoto University and Honda Motor Co.,Ltd.
 * All rights reserved.
 * HARK was developed by researchers in Okuno Laboratory
 * at the Kyoto University and Honda Research Institute Japan Co.,Ltd.
 */

#include <iostream>
#include "BufferedNode.h"
#include "Buffer.h"
#include "Vector.h"
#include <math.h>
#include "Source.h"

using namespace std;
//using namespace boost;
using namespace FD;

class SaveSourceLocation;

DECLARE_NODE(SaveSourceLocation);
/*Node
 *
 * @name SaveSourceLocation
 * @category HARK:Localization
 * @description Save source locations as a text file.
 *
 * @input_name SOURCES
 * @input_type Vector<ObjectRef>
 * @input_description Source locations with ID. Each element of the vector is a source location with ID specified by "Source".
 *
 * @output_name OUTPUT
 * @output_type Vector<ObjectRef>
 * @output_description The same as input.
 *
 * @parameter_name FILENAME
 * @parameter_type string
 * @parameter_description The file name for saving source locations.
 *
 END*/

class SaveSourceLocation : public BufferedNode {
	int sourcesID;
	int outputID;

    string filename;

    ofstream fout;

public:
	SaveSourceLocation(string nodeName, ParameterSet params)
		: BufferedNode(nodeName, params)
    {
        sourcesID = addInput("SOURCES");
        outputID = addOutput("OUTPUT");

        filename = object_cast<string>(parameters.get("FILENAME"));
        if (filename.c_str() == '\0') {
            throw new NodeException(NULL, string("FILENAME is empty."), __FILE__, __LINE__);
        }

        fout.open(filename.c_str());
        if(fout.fail())
        {
            throw new NodeException(NULL, string("Can't open file'")+filename+"'.", __FILE__, __LINE__);
        }

		inOrder = true;
	}

    void calculate(int output_id, int count, Buffer &out)
    {
        RCPtr<Vector<ObjectRef> > pSources = getInput(sourcesID, count);
		const Vector<ObjectRef>& sources = *pSources;

        out[count] = pSources;

        fout << count << " ";
        for (int i = 0; i < sources.size(); i++) {
            RCPtr<Source> src = sources[i];

            fout << src->id << " "
                 << src->x[0] << " "
                 << src->x[1] << " "
                 << src->x[2] << " ";
        }
        fout << endl;
    }
};
