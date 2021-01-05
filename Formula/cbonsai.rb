class Cbonsai < Formula
  desc "Console Bonsai is a bonsai tree generator, written in C using ncurses"
  homepage "https://github.com/mhzawadi/homebrew-cbonsai"
  url "https://github.com/mhzawadi/homebrew-cbonsai/archive/v0.0.1.tar.gz"
  sha256 "5161bdfab3f45f53686ea31e1157c3ae0c59b7bee7d2649f0e06ad5ee88d708c"
  license "GPL-3.0"

  def install
    system "make"
  end

  test do
    system "~/.local/bin/cbonsai", "-p"
  end
end
