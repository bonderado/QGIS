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

#include "qgspointcloudrgbrenderer.h"
#include "qgspointcloudblock.h"

QgsPointCloudRgbRenderer::QgsPointCloudRgbRenderer()
{

}

QString QgsPointCloudRgbRenderer::type() const
{
  return QStringLiteral( "rgb" );
}

QgsPointCloudRenderer *QgsPointCloudRgbRenderer::clone() const
{
  std::unique_ptr< QgsPointCloudRgbRenderer > res = qgis::make_unique< QgsPointCloudRgbRenderer >();
  res->mPenWidth = penWidth();
  res->mRedAttribute = mRedAttribute;
  res->mGreenAttribute = mGreenAttribute;
  res->mBlueAttribute = mBlueAttribute;
  return res.release();
}

void QgsPointCloudRgbRenderer::renderBlock( const QgsPointCloudBlock *block, QgsPointCloudRenderContext &context )
{
  const QgsMapToPixel mapToPixel = context.renderContext().mapToPixel();

  QgsRectangle mapExtent = context.renderContext().mapExtent();

  QPen pen;
  pen.setWidth( mPainterPenWidth );
  pen.setCapStyle( Qt::FlatCap );
  //pen.setJoinStyle( Qt::MiterJoin );

  const char *ptr = block->data();
  int count = block->pointCount();
  const QgsPointCloudAttributeCollection request = block->attributes();

  const std::size_t recordSize = request.pointRecordSize();
  int redOffset = 0;
  const QgsPointCloudAttribute *attribute = request.find( mRedAttribute, redOffset );
  if ( !attribute )
    return;
  const QgsPointCloudAttribute::DataType redType = attribute->type();

  int greenOffset = 0;
  attribute = request.find( mGreenAttribute, greenOffset );
  if ( !attribute )
    return;
  const QgsPointCloudAttribute::DataType greenType = attribute->type();

  int blueOffset = 0;
  attribute = request.find( mBlueAttribute, blueOffset );
  if ( !attribute )
    return;
  const QgsPointCloudAttribute::DataType blueType = attribute->type();

  int rendered = 0;
  double x = 0;
  double y = 0;
  for ( int i = 0; i < count; ++i )
  {
    pointXY( context, ptr, i, x, y );

    if ( mapExtent.contains( QgsPointXY( x, y ) ) )
    {
      int red = 0;
      switch ( redType )
      {
        case QgsPointCloudAttribute::Char:
          continue;

        case QgsPointCloudAttribute::Int32:
          red = *( qint32 * )( ptr + i * recordSize + redOffset );
          break;

        case QgsPointCloudAttribute::Short:
          red = *( short * )( ptr + i * recordSize + redOffset );
          break;

        case QgsPointCloudAttribute::Float:
          red = *( float * )( ptr + i * recordSize + redOffset );
          break;

        case QgsPointCloudAttribute::Double:
          red = *( double * )( ptr + i * recordSize + redOffset );
          break;
      }

      int green = 0;
      switch ( greenType )
      {
        case QgsPointCloudAttribute::Char:
          continue;

        case QgsPointCloudAttribute::Int32:
          green = *( qint32 * )( ptr + i * recordSize + greenOffset );
          break;

        case QgsPointCloudAttribute::Short:
          green = *( short * )( ptr + i * recordSize + greenOffset );
          break;

        case QgsPointCloudAttribute::Float:
          green = *( float * )( ptr + i * recordSize + greenOffset );
          break;

        case QgsPointCloudAttribute::Double:
          green = *( double * )( ptr + i * recordSize + greenOffset );
          break;
      }

      int blue = 0;
      switch ( blueType )
      {
        case QgsPointCloudAttribute::Char:
          continue;

        case QgsPointCloudAttribute::Int32:
          blue = *( qint32 * )( ptr + i * recordSize + blueOffset );
          break;

        case QgsPointCloudAttribute::Short:
          blue = *( short * )( ptr + i * recordSize + blueOffset );
          break;

        case QgsPointCloudAttribute::Float:
          blue = *( float * )( ptr + i * recordSize + blueOffset );
          break;

        case QgsPointCloudAttribute::Double:
          blue = *( double * )( ptr + i * recordSize + blueOffset );
          break;
      }

      mapToPixel.transformInPlace( x, y );

      pen.setColor( QColor( red, green, blue ) );
      context.renderContext().painter()->setPen( pen );
      context.renderContext().painter()->drawPoint( QPointF( x, y ) );

      rendered++;
    }
  }
  context.incrementPointsRendered( rendered );
}


QgsPointCloudRenderer *QgsPointCloudRgbRenderer::create( QDomElement &element, const QgsReadWriteContext & )
{
  std::unique_ptr< QgsPointCloudRgbRenderer > r = qgis::make_unique< QgsPointCloudRgbRenderer >();
  r->setPenWidth( element.attribute( QStringLiteral( "penwidth" ), QStringLiteral( "5" ) ).toInt() );

  r->setRedAttribute( element.attribute( QStringLiteral( "red" ), QStringLiteral( "Red" ) ) );
  r->setGreenAttribute( element.attribute( QStringLiteral( "green" ), QStringLiteral( "Green" ) ) );
  r->setBlueAttribute( element.attribute( QStringLiteral( "blue" ), QStringLiteral( "Blue" ) ) );

  return r.release();
}

QDomElement QgsPointCloudRgbRenderer::save( QDomDocument &doc, const QgsReadWriteContext & ) const
{
  QDomElement rendererElem = doc.createElement( QStringLiteral( "renderer" ) );

  rendererElem.setAttribute( QStringLiteral( "type" ), QStringLiteral( "rgb" ) );
  rendererElem.setAttribute( QStringLiteral( "penwidth" ), mPenWidth );

  rendererElem.setAttribute( QStringLiteral( "red" ), mRedAttribute );
  rendererElem.setAttribute( QStringLiteral( "green" ), mGreenAttribute );
  rendererElem.setAttribute( QStringLiteral( "blue" ), mBlueAttribute );

  return rendererElem;
}

void QgsPointCloudRgbRenderer::startRender( QgsPointCloudRenderContext &context )
{
  QgsPointCloudRenderer::startRender( context );

  mPainterPenWidth = context.renderContext().convertToPainterUnits( mPenWidth, QgsUnitTypes::RenderUnit::RenderMillimeters );
}

void QgsPointCloudRgbRenderer::stopRender( QgsPointCloudRenderContext &context )
{
  QgsPointCloudRenderer::stopRender( context );
}

QSet<QString> QgsPointCloudRgbRenderer::usedAttributes( const QgsPointCloudRenderContext & ) const
{
  return QSet<QString>() << mRedAttribute << mGreenAttribute << mBlueAttribute;
}

int QgsPointCloudRgbRenderer::penWidth() const
{
  return mPenWidth;
}

void QgsPointCloudRgbRenderer::setPenWidth( int value )
{
  mPenWidth = value;
}

QString QgsPointCloudRgbRenderer::redAttribute() const
{
  return mRedAttribute;
}

void QgsPointCloudRgbRenderer::setRedAttribute( const QString &redAttribute )
{
  mRedAttribute = redAttribute;
}

QString QgsPointCloudRgbRenderer::greenAttribute() const
{
  return mGreenAttribute;
}

void QgsPointCloudRgbRenderer::setGreenAttribute( const QString &greenAttribute )
{
  mGreenAttribute = greenAttribute;
}

QString QgsPointCloudRgbRenderer::blueAttribute() const
{
  return mBlueAttribute;
}

void QgsPointCloudRgbRenderer::setBlueAttribute( const QString &blueAttribute )
{
  mBlueAttribute = blueAttribute;
}
