class Cbonsai < Formula
  desc "cbonsai is a bonsai tree generator, written in C using ncurses"
  homepage "https://github.com/mhzawadi/homebrew-cbonsai"
  url "https://github.com/mhzawadi/homebrew-cbonsai/archive/v0.0.1.zip"
  sha256 "eda56875c31ab4dc23d1dbb33155bc30aa3d0d31c0326bd66f3b6f75f55736f4"
  license "GPL-3.0"

  def install
    system "make"
  end

  test do
    system "~/.local/bin/cbonsai", "-p"
  end
end
