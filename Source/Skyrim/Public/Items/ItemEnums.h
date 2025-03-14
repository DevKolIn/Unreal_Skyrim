#pragma once

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon,
	Armor,
	Potion
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	TwoHandedSword,
	OneHandedSword
};

UENUM(BlueprintType)
enum class EArmorType : uint8
{
	Helmet,
	Shield,
	Arm,
	Leg,
	Shoes,
};


UENUM(BlueprintType)
enum class EPotionType : uint8
{
	Heal,
	Mana,
	Stamina,
};

