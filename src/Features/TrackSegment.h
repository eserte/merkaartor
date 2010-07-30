#ifndef MERKATOR_TRACKSEGMENT_H_
#define MERKATOR_TRACKSEGMENT_H_

#include "Feature.h"

class TrackSegmentPrivate;
class Node;

class QProgressDialog;

class TrackSegment : public Feature
{
    Q_OBJECT

    public:
        TrackSegment(void);
        ~TrackSegment(void);
    private:
        TrackSegment(const TrackSegment& other);

        void drawDirectionMarkers(QPainter & P, QPen & pen, const QPointF & FromF, const QPointF & ToF);

    public:
        virtual QString getClass() const {return "TrackSegment";}
        virtual IFeature::FeatureType getType() const {return IFeature::OsmSegment;}
        virtual void updateMeta();

        virtual CoordBox boundingBox() const;
        virtual void draw(QPainter& P, MapView* theView);
        virtual void drawSpecial(QPainter& P, QPen& Pen, MapView* theView);
        virtual void drawParentsSpecial(QPainter& P, QPen& Pen, MapView* theView);
        virtual void drawChildrenSpecial(QPainter& P, QPen& Pen, MapView* theView, int depth);

        virtual double pixelDistance(const QPointF& Target, double ClearEndDistance, bool selectNodes, MapView* theView) const;
        void cascadedRemoveIfUsing(Document* theDocument, Feature* aFeature, CommandList* theList, const QList<Feature*>& Alternatives);
        virtual bool notEverythingDownloaded();
        virtual QString description() const;

        void add(Node* aPoint);
        void add(Node* Pt, int Idx);
        virtual int find(Feature* Pt) const;
        virtual void remove(int idx);
        virtual void remove(Feature* F);
        virtual Feature* get(int idx);
        virtual int size() const;
        Node* getNode(int idx);
        virtual const Feature* get(int Idx) const;
        virtual bool isNull() const;

        void sortByTime();
        virtual void partChanged(Feature* F, int ChangeId);

        double distance();
        int duration() const;

        virtual bool toGPX(QDomElement xParent, QProgressDialog * progress, bool forExport=false);
        static TrackSegment* fromGPX(Document* d, Layer* L, const QDomElement e, QProgressDialog * progress);
        virtual bool toXML(QDomElement xParent, QProgressDialog * progress, bool strict=false);
        static TrackSegment* fromXML(Document* d, Layer* L, const QDomElement e, QProgressDialog * progress);

        virtual QString toHtml() {return "";}

        virtual void toBinary(QDataStream& /* ds */, QHash <QString, quint64>& /*theIndex*/) { return; }

private:
        TrackSegmentPrivate* p;
};

#endif


