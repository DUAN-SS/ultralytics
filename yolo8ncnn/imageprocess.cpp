#include "imageprocess.h"


// 一、灰度线性变化
    // 设源图像的灰度值为x，则进行灰度线性变换后的灰度值为y = kx + b(0 <= y <= 255)。
	/*======================================K > 1==================================================*/
    /* 增加图像的对比度，图像的像素值在进行变换后全部都线性方法，增强了整体的显示效果，            */
	/* 且经过这种变换后，图像的整体对比度明显增大，在灰度图中的体现就是变换后的灰度图明显被拉伸了。*/
	/*=============================================================================================*/

	/*======================================K = 1==================================================*/
    /* 调节图像的亮度，亮度的调节就是让图像的各个像素值都增加或是减少一定量。在这种情况下可以通过* /
	/* 改变d值来达到增加或者是减少图像亮度的目的。因为当k = 1，只改变d值时，只有图像的亮度被改变了.*/
	/* d > 0时，变换曲线整体发生上移，图像的亮度增加，对应的直方图整体向右侧移动，d < 0时，        */
	/* 变换曲线整体下移，图像的亮度降低，对应的直方图发生水平左移。* /
    /*=============================================================================================*/

	/*======================================1 > K > 0==============================================*/
    /* 此时变换的效果正好与k>1时相反，即图像的整体对比度和效果都被削减了，对应的直方图会被集中 * /
	/* 在一段区域上。k值越小，图像的灰度分布也就越窄，图像看起来也就显得越是灰暗。 * /
	/*=============================================================================================*/

	/*======================================K < 0==================================================*/
    /* 源图像的灰度会发生反转，也就是原图像中较亮的区域会变暗，而较暗的区域将会变量。特别的，      */
	/* 此时我们令k = -1, d = 255, 可以令图像实现完全反转的效果。对应的直方图也会发生相应的变化     */

void ContrastStretch(Mat mat, double k, double b)
{
	int RowsNum = mat.rows; 
	int ColsNum = mat.cols;
	Mat dstImg(mat.size(), mat.type());
	//进行遍历图像像素,对每个像素进行相应的线性变换
	for (int i = 0; i < RowsNum; i++) {
		for (int j = 0; j < ColsNum; j++) {
			//c为遍历图像的三个通道
			for (int c = 0; c < 3; c++) {
				//使用at操作符，防止越界
				dstImg.at<Vec3b>(i, j)[c] = saturate_cast<uchar>(k * (mat.at<Vec3b>(i, j)[c]) + b);
			}
		}
	}
	imshow("【灰度调整】线性变换后的图像", dstImg);
}


// 二、灰度对数变换
// y=log(1+x) / b
// b:是一个常数，用来控制曲线的弯曲程度，其中，b越小越靠近y轴，b越大越靠近x轴。
// x:是原始图像中的像素值，y是变换后的像素值，可以分析出，当函数自变量较低时，
// 曲线的斜率很大，而自变量较高时，曲线的斜率变得很小。正是因为对数变换具有这种压缩数据的性质，
// 使得它能够实现图像灰度拓展和压缩的功能。即对数变换可以拓展低灰度值而压缩高灰度级值，
// 让图像的灰度分布更加符合人眼的视觉特征。例如进行傅里叶变换后的图像，
// 图像中心绝对高灰度值的存在压缩了低灰度部分的动态范围，所以无法在现实的时候便显出原油的细节。
// 这时就需要使用一个对数变换来对结果图像进行修正，经过适当的处理后，
// 原始图像中低灰度区域的对比度将会增加，暗部细节将被增强
	// 实现图像的对数变换，作用是压缩图像较亮区域的动态范围
	// 使用不同的方法实现图像的对数变换
	// 基本公式为 y = clog(1+r)

void LogTransformation(Mat srcImage, double c)
{
	Mat srcImage1(srcImage);
	imshow("原图像", srcImage);
	Mat dstImage1(srcImage.size(), srcImage.type());
	Mat dstImage2 = dstImage1.clone();
	Mat dstImage3 = dstImage1.clone();
	//使用第一种方法进行对数变换，对图像整体进行操作
	//首先计算 1+r,注意，是对每一个像素点都进行加1操作
	add(srcImage, Scalar(1.0), srcImage1);
	//转换为32位的浮点数
	srcImage1.convertTo(srcImage1, CV_32F);
	//计算log(1+r)
	log(srcImage1, dstImage1);
	dstImage1 = c * dstImage1;
	//进行归一化处理
	normalize(dstImage1, dstImage1, 0, 255, NORM_MINMAX);
	//convertScaleAbs:先缩放元素再取绝对值，最后转换格式为8bit型
	//在这里不具有缩放功能，作用仅为将格式转换为8bit型
	convertScaleAbs(dstImage1, dstImage1);
	imshow("对数变换图像1", dstImage1);
	//	//使用第二种方法进行图像的对数变换，对图像的像素进行遍历
	//	double temp = 1;
	//for (int i = 0; i < srcImage.rows; i++)
	//{
	//	for (int j = 0; j < srcImage.cols; j++)
	//	{
	//		temp = (double)srcImage.at<uchar>(i, j);
	//		temp = c * log((double)(1 + temp));
	//		dstImage2.at<uchar>(i, j) = saturate_cast<uchar>(temp);
	//	}
	//}
	////进行归一化处理
	//normalize(dstImage2, dstImage2, 0, 255, NORM_MINMAX);
	//convertScaleAbs(dstImage2, dstImage2);
	//imshow("对数变换图像2", dstImage2);
	////
	////使用第三种方法进行图像的对数变换
	////首先进行图像类型转换
	//srcImage.convertTo(dstImage3, CV_32F);
	////图像矩阵元素进行加1操作
	//dstImage3 = dstImage3 + 1;
	////图像对数操作
	//cv::log(dstImage3, dstImage3);
	//dstImage3 = c * dstImage3;
	////图像进行归一化操作
	//normalize(dstImage3, dstImage3, 0, 255, NORM_MINMAX);
	//convertScaleAbs(dstImage3, dstImage3);
	//imshow("对数变换图像3", dstImage3);
	//waitKey();
}

//三、幂次变换与Gamma灰度校正
//基于幂次变换的Gamma校正是图像处理中一种非常重要的非线性变换，它与对数变换相反，
//它是对输入图像的灰度值进行指数变换，进而校正亮度上的偏差。通常Gamma校正长应用于拓展暗调的细节。
//通常来讲，当Gamma校正的值大于1时，图像的高光部分被压缩而暗调部分被扩展；
//当Gamma校正的值小于1时，相反的，图像的高光部分被扩展而暗调备份被压缩。
//通常情况下，最简单的Gamma校正可以用下面的幂函数来表示： v(out) = AV(γ)in
//  其中A是常数，函数的输入和输出都是非负数，
//  r = 1时，为直线变换；
//  r < 1时，低灰度区域动态范围扩大，进而图像对比度增强，高灰度值区域动态范围减小，图像对比度降低，图像整体灰度值增大，此时与图像的对数变换类似。
//	r>11时，低灰度区域的动态范围减小进而对比度降低，高灰度区域动态范围扩大，图像的对比度提升，图像的整体灰度值变小，Gamma校正主要应用在图像增强。目标检测和图像分析等不同的领域。
//	总之，
//	r < 1的幂函数的作用是提高图像暗区域中的对比度，而降低亮区域的对比度；
//	r>1的幂函数的作用是提高图像中亮区域的对比度，降低图像中按区域的对比度。
//	所以，对于灰度级整体偏暗的图像，可以使用r < 1的幂函数增大动态范围。对于灰度级整体偏亮的图像，
//	可以使用r>1的幂函数增大灰度动态范围。

Mat GammaTrans(Mat srcImag, float parameter)
{
	//建立查表文件LUT
	unsigned char LUT[256];
	for (int i = 0; i < 256; i++)
	{
		//Gamma变换定义
		LUT[i] = saturate_cast<uchar>(pow((float)(i / 255.0), parameter) * 255.0f);
	}
	Mat dstImage = srcImag.clone();
	//输入图像为单通道时，直接进行Gamma变换
	if (srcImag.channels() == 1)
	{
		MatIterator_<uchar>iterator = dstImage.begin<uchar>();
		MatIterator_<uchar>iteratorEnd = dstImage.end<uchar>();
		for (; iterator != iteratorEnd; iterator++)
			*iterator = LUT[(*iterator)];
	}
	else
	{
		//输入通道为3通道时，需要对每个通道分别进行变换
		MatIterator_<Vec3b>iterator = dstImage.begin<Vec3b>();
		MatIterator_<Vec3b>iteratorEnd = dstImage.end<Vec3b>();
		//通过查表进行转换
		for (; iterator != iteratorEnd; iterator++)
		{
			(*iterator)[0] = LUT[((*iterator)[0])];
			(*iterator)[1] = LUT[((*iterator)[1])];
			(*iterator)[2] = LUT[((*iterator)[2])];
		}
	}
	imshow("Gamma变换后的图像", dstImage);
	return dstImage;
}


//四、分段线性变换
// 分段线性变换也是一种重要的灰度级变换。对于曝光不足，曝光过度和传感器动态范围都会造成图像表现出低对比度的特征。
// 分段线性变换的作用是提高图像灰度级的动态范围。通常来说，通过阶段一定比例的最亮像素和最暗像素，
// 并使得中间亮度像素占有整个灰度级，因而能够提高图像的全局对比度。在这种情况下，通常称之为对比度拉伸，直方图裁剪，
// 目前广泛的应用于图像后期处理中。通常使用分段函数来实现。下面先简单介绍一下对比度拉伸技术。
// 图像的对比度拉伸是通过扩展图像灰度级动态范围来实现的，它可以扩展对应的全部灰度范围。
// 图像的低对比度一般是由于图像图像成像亮度不够、成像元器件参数限制或设置不当造成的。
// 提高图像的对比度可以增强图像各个区域的对比效果，对图像中感兴趣的区域进行增强，
// 而对图像中不感兴趣的区域进行相应的抑制作用。对比度拉伸是图像增强中的重要的技术之一。
// 这里设点(x1, y1)与(x2, y2)是分段线性函数中折点位置坐标。常见的三段式分段线性变换函数的公式如下
				/*=================================================*/
				//         k1x                       x < x1        |
				//	f(x) = k2(x - x1) + y2       x1 <= x <= x2     |
				//	       k3(x - x2) + y2             x < x2      |
				/*=================================================*/
// 其中:
		/****************************************************************************/
		/** k1 = y1 / x1, k2 = (y2 - y1) / (x2 - x1), k3 = (255 - y2) / (255 - y1) **/
		/** *************************************************************************/

// 需要注意的是，分段线性一般要求函数是单调递增的，目的是防止图像中的灰度级不满足一一映射。
// 分段的灰度拉伸技术可以结合直方图处理技术，从而更加灵活地控制输出图像的直方图分布,
// 对特定感兴趣的区域进行对比度调整，增强图像画质。对于图像灰度集中在较暗的区域，可以采用斜率k < 0来进行灰度拉伸扩展;
// 对于图像中较亮的区域，可以采用修了k < 0来进行灰度拉伸压缩
void GrayScaleStretch(Mat srcImage)
{
	Mat dstImage(srcImage);
	int rowsNum = dstImage.rows;
	int colsNum = dstImage.cols;
	//图像连续性判断
	if (dstImage.isContinuous())
	{
		colsNum = colsNum * rowsNum;
		rowsNum = 1;
	}
	//图像指针操作
	uchar* pDataMat;
	int pixMax = 0, pixMin = 255;
	//计算图像像素的最大值和最小值
	for (int j = 0; j < rowsNum; j++)
	{
		pDataMat = dstImage.ptr<uchar>(j);
		for (int i = 0; i < colsNum; i++)
		{
			if (pDataMat[i] > pixMax)
				pixMax = pDataMat[i];
			if (pDataMat[i] < pixMin)
				pixMin = pDataMat[i];
		}
	}

	//进行对比度拉伸
	for (int j = 0; j < rowsNum; j++)
	{
		pDataMat = dstImage.ptr<uchar>(j);
		for (int i = 0; i < colsNum; i++)
		{
			pDataMat[i] = (pDataMat[i] - pixMin) * 255 / (pixMax - pixMin);
		}
	}
	imshow("对比度拉伸后的图像", dstImage);
}


// 五、实现灰度级分层
// 灰度级分层，也叫做灰度级切片，作用是在整个灰度级范围内将设定窗口内的灰度和其他部分分开。
// 从而突出图像中具有一定灰度范围的区域。大体上来说，灰度级分层有两种类型，
// 即：清除背景和保持背景。
// 清除背景是将灰度窗口内的像素赋值为较亮的值，而其他部分赋值为较暗的值。
// 经过这样的处理后产生的是二值图像，原图像的细节将全部丢失。
// 而保持背景指的是将灰度窗口内的像素赋值为较亮的值，而其他部分的灰度保持不变。
void GreyScale(Mat srcImage)
{
	Mat dstImage = srcImage.clone();
	int rowsNum = dstImage.rows;
	int colsNum = dstImage.cols;
	//图像连续性判断
	if (dstImage.isContinuous())
	{
		colsNum *= rowsNum;
		rowsNum = 1;
	}
	//图像指针操作
	uchar* pDataMat;
	int controlMin = 50;
	int controlMax = 50;
	//计算图像的灰度级分层
	for (int j = 0; j < rowsNum; j++)
	{
		pDataMat = dstImage.ptr<uchar>(j);
		for (int i = 0; i < colsNum; i++)
		{
			//第一种方法，二值映射
			if (pDataMat[i] > controlMin)
				pDataMat[i] = 255;
			else
				pDataMat[i] = 0;
			////第二种方法：区域映射
			//if (pDataMat[i] > controlMax && pDataMat[j] < controlMin)
			//	pDataMat[i] = controlMax;
		}
	}
	imshow("灰度分层后的图像", dstImage);
}