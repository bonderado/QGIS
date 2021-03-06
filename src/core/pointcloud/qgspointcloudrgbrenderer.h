/***************************************************************************
                         qgspointcloudrgbrenderer.h
                         --------------------
    begin                : October 2020
    copyright            : (C) 2020 by Nyall Dawson
    email                : nyall dot dawson at gmail dot com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSPOINTCLOUDRGBRENDERER_H
#define QGSPOINTCLOUDRGBRENDERER_H

#include "qgspointcloudrenderer.h"
#include "qgis_core.h"
#include "qgis_sip.h"

/**
 * \ingroup core
 * An RGB renderer for 2d visualisation of point clouds using embedded red, green and blue attributes.
 *
 * \since QGIS 3.18
 */
class CORE_EXPORT QgsPointCloudRgbRenderer : public QgsPointCloudRenderer
{
  public:

    /**
     * Constructor for QgsPointCloudRgbRenderer.
     */
    QgsPointCloudRgbRenderer();
    QString type() const override;
    QgsPointCloudRenderer *clone() const override;
    void renderBlock( const QgsPointCloudBlock *block, QgsPointCloudRenderContext &context ) override;
    QDomElement save( QDomDocument &doc, const QgsReadWriteContext &context ) const override;
    void startRender( QgsPointCloudRenderContext &context ) override;
    void stopRender( QgsPointCloudRenderContext &context ) override;
    QSet< QString > usedAttributes( const QgsPointCloudRenderContext &context ) const override;

    /**
     * Creates an RGB renderer from an XML \a element.
     */
    static QgsPointCloudRenderer *create( QDomElement &element, const QgsReadWriteContext &context ) SIP_FACTORY;

    //! Returns pen width
    int penWidth() const;

    //! Sets pen width
    void setPenWidth( int value );

    /**
     * Returns the attribute to use for the red channel.
     *
     * \see greenAttribute()
     * \see blueAttribute()
     * \see setRedAttribute()
     */
    QString redAttribute() const;

    /**
     * Sets the \a attribute to use for the red channel.
     *
     * \see setGreenAttribute()
     * \see setBlueAttribute()
     * \see redAttribute()
     */
    void setRedAttribute( const QString &attribute );

    /**
     * Returns the attribute to use for the green channel.
     *
     * \see redAttribute()
     * \see blueAttribute()
     * \see setGreenAttribute()
     */
    QString greenAttribute() const;

    /**
     * Sets the \a attribute to use for the green channel.
     *
     * \see setRedAttribute()
     * \see setBlueAttribute()
     * \see greenAttribute()
     */
    void setGreenAttribute( const QString &attribute );

    /**
     * Returns the attribute to use for the blue channel.
     *
     * \see greenAttribute()
     * \see redAttribute()
     * \see setBlueAttribute()
     */
    QString blueAttribute() const;

    /**
     * Sets the \a attribute to use for the blue channel.
     *
     * \see setRedAttribute()
     * \see setGreenAttribute()
     * \see blueAttribute()
     */
    void setBlueAttribute( const QString &attribute );

  private:
    int mPenWidth = 1;
    int mPainterPenWidth = 1;

    QString mRedAttribute = QStringLiteral( "Red" );
    QString mGreenAttribute = QStringLiteral( "Green" );
    QString mBlueAttribute = QStringLiteral( "Blue" );

};

#endif // QGSPOINTCLOUDRGBRENDERER_H
