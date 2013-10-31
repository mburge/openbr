#include "openbr_internal.h"
#include <FileStorageModel.hpp>
#include <PartsBasedDetector.hpp>
#include <Candidate.hpp>

using namespace cv;

namespace br
{

class PBDResourceMaker : public ResourceMaker<PartsBasedDetector<double> >
{
public:
    PBDResourceMaker(QString modelFile) : ResourceMaker<PartsBasedDetector<double> >()
    {
        this->modelFile = modelFile;
    }

private:
    QString modelFile;

    PartsBasedDetector<double> *make() const
    {
        FileStorageModel model;
        PartsBasedDetector<double> *pbd = new PartsBasedDetector<double>();
        model.deserialize(modelFile.toStdString());
        pbd->distributeModel(model);
        return pbd;
    }
};

/*!
 * \ingroup transforms
 * \brief Detects objects using a parts-based approach.
 * \author Austin Blanton \cite imaus10
 */
class PartsBasedDetectTransform : public UntrainableTransform
{
    Q_OBJECT
    Q_PROPERTY(QString modelFile READ get_modelFile WRITE set_modelFile RESET reset_modelFile STORED false)
    BR_PROPERTY(QString, modelFile, Globals->sdkPath + "/share/openbr/models/PartsBasedDetector/PersonINRIA_9parts.xml")
    Resource<PartsBasedDetector<double> > pbdResource;

    void init()
    {

        pbdResource.setResourceMaker(new PBDResourceMaker(modelFile));
    }

    void project(const Template &src, Template &dst) const
    {
        dst = src;
        PartsBasedDetector<double> *pbd = pbdResource.acquire();
        std::vector<Candidate> candidates;
        pbd->detect(src, candidates);
        QList<float> confidences = QList<float>();
        foreach (const Candidate &candi, candidates) {
            dst.file.appendRect(candi.boundingBox());
            confidences.append(candi.score());
        }
        dst.file.setList<float>("Confidences", confidences);
        pbdResource.release(pbd);
    }
};

BR_REGISTER(Transform, PartsBasedDetectTransform)

} // namespace br

#include "wgpartsbaseddetector.moc"
