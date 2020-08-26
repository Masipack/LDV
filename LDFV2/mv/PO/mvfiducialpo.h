#ifndef MVFIDUCIALPO_H
#define MVFIDUCIALPO_H

#include <QObject>
#include <QtCore>
#include <opencv2/core.hpp>

class MvFIDUCIALPO: public QObject
{
    Q_OBJECT
public:
    explicit MvFIDUCIALPO(QObject *parent = nullptr);
    virtual ~MvFIDUCIALPO();

    void SetStartPosition(QPointF position){ startPosition = position;}
    void SetMask(cv::Mat value_mask){ mask = value_mask;}


signals:
    void ExecResult(const double& correlation_value,QPointF& currrent_position, quint32 prid);
    void Result(const QString& text, quint32 prid);

public slots:
    void Exec(const cv::Mat& roi, quint32 proc_id);


private:

    cv::Mat mask;
    QPointF startPosition;
protected:
    double correlation(cv::Mat& image_1, cv::Mat& image_2);

};

#endif // MVFIDUCIALPO_H
