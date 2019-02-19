// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Styling/ButtonWidgetStyle.h"
#include "CUBICARButtonWidgetStyle.generated.h"

/**
 * Represents the appearance of an SButton
 */
USTRUCT(BlueprintType)
struct CUBICAR_API FCUBICARButtonStyle : public FButtonStyle
{
	GENERATED_USTRUCT_BODY()

		FCUBICARButtonStyle();

	virtual ~FCUBICARButtonStyle() = default;

	virtual void GetResources(TArray< const FSlateBrush* >& OutBrushes) const override;

	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };

	static const FCUBICARButtonStyle& GetDefault();

	/** Font family and size to be used when displaying this text. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance)
		FSlateFontInfo Font;
	FCUBICARButtonStyle& SetFont(const FSlateFontInfo& InFont) { Font = InFont; return *this; }
	FCUBICARButtonStyle& SetFont(TSharedPtr<const FCompositeFont> InCompositeFont, const int32 InSize, const FName& InTypefaceFontName = NAME_None) { Font = FSlateFontInfo(InCompositeFont, InSize, InTypefaceFontName); return *this; }
	FCUBICARButtonStyle& SetFont(const UObject* InFontObject, const int32 InSize, const FName& InTypefaceFontName = NAME_None) { Font = FSlateFontInfo(InFontObject, InSize, InTypefaceFontName); return *this; }
	FCUBICARButtonStyle& SetFont(const FName& InFontName, uint16 InSize) { Font = FSlateFontInfo(InFontName, InSize); return *this; }
	FCUBICARButtonStyle& SetFont(const FString& InFontName, uint16 InSize) { Font = FSlateFontInfo(*InFontName, InSize); return *this; }
	FCUBICARButtonStyle& SetFont(const WIDECHAR* InFontName, uint16 InSize) { Font = FSlateFontInfo(InFontName, InSize); return *this; }
	FCUBICARButtonStyle& SetFont(const ANSICHAR* InFontName, uint16 InSize) { Font = FSlateFontInfo(InFontName, InSize); return *this; }
	FCUBICARButtonStyle& SetFontName(const FName& InFontName) { Font = FSlateFontInfo(InFontName, Font.Size); return *this; }
	FCUBICARButtonStyle& SetFontName(const FString& InFontName) { Font = FSlateFontInfo(InFontName, Font.Size); return *this; }
	FCUBICARButtonStyle& SetFontName(const WIDECHAR* InFontName) { Font = FSlateFontInfo(InFontName, Font.Size); return *this; }
	FCUBICARButtonStyle& SetFontName(const ANSICHAR* InFontName) { Font = FSlateFontInfo(InFontName, Font.Size); return *this; }
	FCUBICARButtonStyle& SetFontSize(uint16 InSize) { Font.Size = InSize; return *this; }
	FCUBICARButtonStyle& SetTypefaceFontName(const FName& InTypefaceFontName) { Font.TypefaceFontName = InTypefaceFontName; return *this; }

	/** The color and opacity of this text */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (DisplayName = "Color"))
		FSlateColor ColorAndOpacity;
	FCUBICARButtonStyle& SetTextColorAndOpacity(const FSlateColor& InColorAndOpacity) { ColorAndOpacity = InColorAndOpacity; return *this; }

	/** How much should the shadow be offset? An offset of 0 implies no shadow. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance)
		FVector2D ShadowOffset;
	FCUBICARButtonStyle& SetTextShadowOffset(const FVector2D& InShadowOffset) { ShadowOffset = InShadowOffset; return *this; }

	/** The color and opacity of the shadow */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance)
		FLinearColor ShadowColorAndOpacity;
	FCUBICARButtonStyle& SetTextShadowColorAndOpacity(const FLinearColor& InShadowColorAndOpacity) { ShadowColorAndOpacity = InShadowColorAndOpacity; return *this; }

	/** The shape of highlighted text */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, AdvancedDisplay)
		FSlateBrush HighlightShape;
	FCUBICARButtonStyle& SetHighlightShape(const FSlateBrush& InHighlightShape) { HighlightShape = InHighlightShape; return *this; }

	/** The brush used to draw an underline under the text (if any) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, AdvancedDisplay)
		FSlateBrush UnderlineBrush;
	FCUBICARButtonStyle& SetUnderlineBrush(const FSlateBrush& InUnderlineBrush) { UnderlineBrush = InUnderlineBrush; return *this; }
};


/**
 * 
 */
UCLASS()
class CUBICAR_API UCUBICARButtonWidgetStyle : public USlateWidgetStyleContainerBase
{
public:
	GENERATED_BODY()

public:
	/** The actual data describing the button's appearance. */
	UPROPERTY(Category = Appearance, EditAnywhere, BlueprintReadWrite, meta = (ShowOnlyInnerProperties))
		FCUBICARButtonStyle ButtonStyle;



	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast<const struct FSlateWidgetStyle*>(&ButtonStyle);
	}


	/**
	 *	@fn SetTextStyle()
	 *	@brief Sets the text bloc according to the style 
	 *	@param Button : the custom button to skin.
	 *	@param WidgetStyle : the style to use
	 */
	static void SetStyle(class UCUBICARButton* Button, const FCUBICARButtonStyle& WidgetStyle);
	
};
